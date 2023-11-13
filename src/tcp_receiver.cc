#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;


void TCPReceiver::segment_received(const TCPSegment &seg) {

    auto header = seg.header();
    auto data = seg.payload();
    bool closeInput = false;

   
    if (header.syn && _synReceived || header.fin && _finReceived) {
        return;
    }

 
    if (header.syn && !_synReceived) {
        _isn = header.seqno;
        nextSeqno= unwrap(_isn+1,_isn,_isn.raw_value());
       
        _synReceived=true;
        raw = nextSeqno;
    }


    if (header.fin && !_finReceived) {
        _finReceived=true;
        closeInput = true;

    }

    if (data.size()==0) {

        if (closeInput) {
            stream_out().end_input();
            nextSeqno++;
        }
        return;
      
    }

    auto SeqLeft = unwrap(header.seqno,_isn,nextSeqno); 
    auto SeqRight = SeqLeft+data.size()-1; 
    auto WindowLeft = nextSeqno; 
    auto WindowRight=nextSeqno+window_size()-1; 
    auto MaxLeft = max(SeqLeft,WindowLeft); 
    auto MinRight = min(SeqRight,WindowRight); 
    auto skipBytesLeft =MaxLeft - SeqLeft; 
    auto skipBytesRight = SeqRight - MinRight; 

    if (skipBytesLeft+skipBytesRight>=data.size()) {
        
        return;
    }

    
    auto bytesInWindow = data.str().substr(skipBytesLeft,data.size()-skipBytesRight);

    auto beforeWritten = stream_out().bytes_written();

    _reassembler.push_substring(static_cast<std::string>(bytesInWindow),MaxLeft-raw,false);

    
    nextSeqno += (stream_out().bytes_written() - beforeWritten ); 

    if (closeInput) {
        stream_out().end_input();
        nextSeqno++;
    }
    return;
        


}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_synReceived) {
        return wrap(nextSeqno,_isn);
    }
    return optional<WrappingInt32>{};
}


size_t TCPReceiver::window_size() const {
    //cap - bytesinstream
    return stream_out().remaining_capacity();
}