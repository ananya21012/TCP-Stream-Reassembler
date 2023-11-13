#include "stream_reassembler.hh"

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;



StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity), buffer()
{
    _capacity=capacity;
    output_len=-1;
    current_index=0;
    _unassembled=0;
   
}


size_t StreamReassembler::unacceptable() { return _output.bytes_read() + _capacity; }


//! \details This function does the real work of manipulating member unassembled.
//! \param[in] data mustn't overlap with out terms in unassembled, except for
//! the one with same index
void StreamReassembler::push(const std::string &data, const size_t index) {
    auto it = buffer.find(index);
    if(it == buffer.end()) {
        _unassembled += data.size();
        buffer.insert({index, move(data)});
    } else if (!data.empty() && data.size() > it->second.size()) {
        _unassembled += data.size() - it->second.size();
        it->second = move(data);
    }
}

//! \details This function accecpts a substring, check whether the substring has already been
//! written or beyound the scope of capacity. If that is true, then return false; otherwise
//! return true. If the data has been partially written or beyound the scope, then trim it
//! and return true.
bool StreamReassembler::isvalid(std::string &data, size_t &index) {
    // return true if [first_unassembled(), first_unacceptable()).
   
    if(index >= unacceptable()) return false;
    if(index + data.size() < _output.bytes_written()) return false;

    // trim back
    if(index < unacceptable() && index + data.size() >= unacceptable()) {
        data = move(data.substr(0, unacceptable() - index));
    }

    // trim front
    if(index < _output.bytes_written() && index + data.size() >= _output.bytes_written()) {
        data = move(data.substr(_output.bytes_written() - index));
        index = _output.bytes_written();
    }

    return true;
}



//! \details This function remove all the duplicated part of \param[in] data
//! and call push_available to do the real work of pushing the substring.
//! \param[in] data is guaranteed to be in legal range of capacity.
void StreamReassembler::push_in_buffer(string data, size_t index) {

    std::map<size_t, string>::iterator it = buffer.lower_bound(index);
    if(it != buffer.begin()) { // former term exists
        --it;
        const auto &[idex, size] = make_pair(it->first, it->second.size());

        if(index + data.size() < idex + size) return;
        if(index < idex + size) {
            data = move(data.substr(idex + size - index));
            index = idex + size;
        }
    }
    
    if(data.empty()) {
        push(data, index);
        return;
    }

    auto hi = buffer.lower_bound(index + data.size());
    for(it = buffer.upper_bound(index); it != hi; it++) {
        const auto &[idex, size] = make_pair(it->first, it->second.size());
        // index <= idx holds in any condition
        if(index + data.size() <= idex + size) {
            data = move(data.substr(0, idex - index));
        } else {
            push(move(data.substr(0, idex - index)), index);
            data = move(data.substr(idex + size - index));
            index = idex + size;
        }
    }
    push(move(data), index);
}

void StreamReassembler::assemble() {
    _unassembled -= buffer.begin()->second.size();
    buffer.erase(buffer.begin());
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {

    if(_output.input_ended()) return;
    if(eof) output_len = index + data.size();

    string strg = move(data);
    size_t idex = move(index); 

    if(!isvalid(strg, idex)) return;
    push_in_buffer(move(strg), idex);

    while(!buffer.empty() && buffer.begin()->first == _output.bytes_written()) {
        _output.write(move(buffer.begin()->second));
        assemble();
        if(_output.bytes_written() == output_len) {
            _output.end_input();
            break;
        }
    }
}



size_t StreamReassembler::unassembled_bytes() const { return _unassembled; }

bool StreamReassembler::empty() const { return (buffer.empty() && _output.buffer_empty()); }

size_t StreamReassembler::ack_index() const { return current_index + output_len; } //