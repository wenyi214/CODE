#pragma once

#include"Sock.hpp"
#include"Log.hpp"

class Util{
  public:
    
    static void CutRequestLine(const std::string line,std::string& method,std::string& url,std::string& version){
      
      std::stringstream ss(line);
      //以空格为分隔符，依次输出到method,url,version中
      ss >> method >> url >> version;
      //Log(Notice, line);
      //Log(Notice, method);
      //Log(Notice, url);
      //Log(Notice, version);

    }
    static void CutRequestHeaderToKV(const std::string& line, std::string& k, std::string& v){
      
      size_t pos = line.find(": ");
      if(pos != std::string::npos){
        k = line.substr(0, pos);
        v = line.substr(pos+2);

        Log(Notice, k);
        Log(Notice, v);
   
      }
    }
    static ssize_t StringToInt(const std::string& v){
      
      std::stringstream ss(v);
      ssize_t i = 0;
      ss >> i;
      return i;
    }
};
