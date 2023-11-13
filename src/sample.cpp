//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.


void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {

    if(index==current_index){
        _output.write(data);
        current_index+=min(data.length(),_capacity);
          string temp="";

        while(!buffer.empty() && buffer.find(current_index)!= buffer.end()){
            temp.push_back(buffer[current_index]);
            buffer.erase(current_index);
            current_index++;
        }
         if (!temp.empty()) {
            _output.write(temp);
        }

    }
   
    else{
      
       
        int i=0;
        if(index>current_index){

         if(index <= _capacity && i<data.length()){
            char c=data[i];
            buffer.insert({index+i,c});

            i++;}
            unassembled=buffer.size();
        }
        
    }
    if(eof){
        
    }
    if (eof) {
       _output.end_input();
    }


    //3 cases and overwrite

}