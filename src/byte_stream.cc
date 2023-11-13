
#include "byte_stream.hh"

#include <algorithm>
#include<iostream>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa)
{ 
	capacity=capa;
	_inputEnded=false;
	_bytesWritten=0;
	_bytesRead=0;
	_error=false;
}

//size_t get_capacity(){
  //return this.capacity;
//}

size_t ByteStream::write(const string &data) {
	if(_inputEnded || _error){
		return 0;
	}
  int bytes= min(remaining_capacity(),data.length());
	_bytesWritten+=bytes;
	
	for(int i=0;i<bytes;i++){
		stream.push_back(data[i]);
	}

	return bytes;
}
//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
	  string s="";
	  if(len>stream.size()){
	  	 for(int i=0;i<stream.size();i++){
		  	s+=stream.at(i);
		}
	  }
	  else{
	  
		  for(int i=0;i<len;i++){
		  s+=stream.at(i);
		  }
		}
	  
	  return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
	if(stream.size()>=len){
		stream.erase(stream.begin(),stream.begin()+len);
		_bytesRead+=len;
	}
	else{
		_bytesRead+=stream.size();
		stream.clear();}
	

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  std::string read_string=peek_output(len);
  pop_output(len);

  return read_string;
}

void ByteStream::end_input() {_inputEnded=true; }

bool ByteStream::input_ended() const { return _inputEnded;}

size_t ByteStream::buffer_size() const {return stream.size(); }

bool ByteStream::buffer_empty() const {return stream.empty(); }

bool ByteStream::eof() const { return _inputEnded && stream.empty(); }

size_t ByteStream::bytes_written() const { return _bytesWritten; }

size_t ByteStream::bytes_read() const { return _bytesRead; }

size_t ByteStream::remaining_capacity() const { 

	return (capacity-stream.size()); }


  // 1 testcase failed