#pragma once
#include"Sock.hpp"
#include"Util.hpp"
#include"Log.hpp"

#define WEBROOT "wwwroot"
#define HOMEPAGE "text.html"
#define WEBVERSION "HTTP/1.0"
//具体进行解析请求
class HttpRequest{
  private:
    std::string request_line;
    std::vector<std::string> request_header;
    std::string blank;
    std::string request_body;
  private:
    std::string method;//请求方法
    std::string url;
    std::string version;//http版本

  private:
    std::unordered_map<std::string,std::string> header_kv;//报头kv保存，方便查找
    int content_length;//正文字节数 -1没有正文
    //与分析有关
    std::string path;     //请求资源路径
    std::string parameter;//参数

    //POST有正文，GET有参数，可执行程序使用cgi技术
    bool cgi;//是否cgi技术
    ssize_t filesize;//文件大小
    
    std::string suffix;
  public:
    HttpRequest()
      :blank("\n")
      ,content_length(-1)
      ,path(WEBROOT)
      ,cgi(false)
      ,filesize(0)
      ,suffix("text/html")
    {}
    void SetCgi(){
      cgi = true;
    }
    bool IsCgi(){
      return cgi;
    }
    std::string GetPath(){
      return path;
    }
    void SetPath(std::string p){
      path = p;
    }
    std::string GetMethod(){
      return method;
    }
    std::string GetParameter(){
      return parameter;
    }
    //请求行
    void SetRequestLine(const std::string& line){
      request_line = line;
    }
    void ParseRequestLine(){
      //分割请求行，获得方法，url，版本
      Util::CutRequestLine(request_line, method, url, version);
    }
    //请求报头
    void SetRequestHeader(const std::string& line){
      request_header.push_back(line);
      //Log(Notice, line);
    }
    std::string GetBody(){
      return request_body;
    }
    ssize_t GetFileSize(){
      return filesize;
    }
    void SetFileSize(ssize_t fsize){
      filesize = fsize;
    }

    void ParseRequestHeader(){
      std::string k,v;
      for(const auto& ch : request_header){
        Util::CutRequestHeaderToKV(ch,k,v);
        //header_kv[k] = v;
        header_kv.insert({k,v});
        //获得content_length
        if(k == "Content-Length"){
          //将V转化为字符串返回
          content_length = Util::StringToInt(v);
          Log(Notice, v);
        }
      }

    }
    //判断是否需要读取正文
    bool IsGetRequestBody(){
      //请求方法是否是POST，正文长度是否大于0
      return strcasecmp(method.c_str(), "POST") == 0 && content_length > 0;
    }
    ssize_t GetContenLength(){
      return content_length;
    }
    void SetRequestBody(const std::string& body){
      //POST有正文
      cgi = true;
      request_body = body;
      Log(Notice,body);
    }
    bool IsLegalMethod(){
      if(strcasecmp(method.c_str(), "GET") == 0 || strcasecmp(method.c_str(), "POST") == 0){
        return true;
      }
      return false;
    }
    bool IsGetMethod(){
      return strcasecmp(method.c_str(), "GET") == 0;
    }
    bool IsPostMethod(){
      return strcasecmp(method.c_str(), "POST") == 0;
    }
    void ParseRequestUrl(){
      size_t pos = url.find('?');
      if(pos == std::string::npos){
        path += url;
      }
      else{
        //GET有参数
        cgi = true;
        path += url.substr(0, pos);
        parameter = url.substr(pos+1);
      }
    }
    void UrlSetToPath(){
      path += url;
    }
    void AddHomePage(){
      if(path[path.size() -1] == '/'){
        path += HOMEPAGE;
      }
    }
    std::string MakeSuffix(){
        //通过申请资源后缀确定content_type
        size_t pos = path.rfind(".");
        if(pos != std::string::npos){
          suffix = path.substr(pos+1);
        }
        return suffix;

    }
    ~HttpRequest()
    {}

};
//具体进行组合响应
class HttpResponse{
  private:
    std::string response_line;
    std::vector<std::string> response_header;
    std::string blank;
    std::string response_body;

  public:
    HttpResponse()
    :blank("\r\n")
    {}
    void SetResponseLine(const std::string& rsp_line){
      response_line = rsp_line;
    }
    std::string GetResponseLine(){
      return response_line;
    }
    void SetResponseHeader(std::vector<std::string>& rsp_header){
      response_header = rsp_header;
    }
    std::vector<std::string> GetResponseHeader(){
      //最后加一个空行
      response_header.push_back(blank);
      return response_header;
    }
    void AddHeader(std::string& content_type){
      response_header.push_back(content_type);  
    }
    ~HttpResponse()
    {}

};
static std::string CodeToDescribe(int code){
  std::string desc;
  switch(code){
    case 200:
      desc = "OK";
      break;
    case 404:
      desc = "Not Found";
      break;
    default:
      desc = "OK";
      break;
  }
  return desc;
}
static std::string SuffixDesc(std::string& suffix){
  if(suffix == "html"){
    return "text/html"; 
  }
  else if(suffix == "css"){
    return "text/css";
  }
  else if(suffix == "js"){
    return "application/x-javascript";
  }
  else{
    return "text/html";
  }
}
//进行通信，收请求，发响应,提供接收请求，解析请求，组合响应，发送响应接口
class EndPoint{
  private:
    int sock;
    HttpRequest req;
    HttpResponse rsp;

  private:
    
    void GetRequestLine(){
      std::string line;
      Sock::GetLine(sock, line);
      req.SetRequestLine(line);
      //分析
      req.ParseRequestLine();

    }
    void GetRequestHeader(){
      std::string line;
      do{
        //清空
        line = "";
        Sock::GetLine(sock,line);
        req.SetRequestHeader(line);
      }while(!line.empty());

      req.ParseRequestHeader();

    }
    void GetRequestBody(){
      std::string body;
      ssize_t len = req.GetContenLength();
      char c;
      while(len){
        recv(sock, &c, 1, 0);
        body.push_back(c);
        len--;
      }
      req.SetRequestBody(body);
      
    }
      //构建响应行
      void MakeResponseLine(int code){
        std::string rsp_line;
        rsp_line = WEBVERSION;
        rsp_line += " ";
        rsp_line += std::to_string(code);
        rsp_line += " ";
        rsp_line += CodeToDescribe(code);
        rsp_line += "\r\n";
        rsp.SetResponseLine(rsp_line);
      }
      //构建响应报头
      void MakeReponseHeader(){
        //申请一个cgi程序没有后缀
        std::string suffix = req.MakeSuffix();
        std::string content_type = "Content-type: ";
        content_type += SuffixDesc(suffix);
        content_type += "\r\n";
        rsp.AddHeader(content_type);

      }
      //非CGI方式处理
      void EchoNoCgiExe(std::string& path){
        //读文件，读到0或者小于或者结束
        Log(Notice, path.c_str());
        int fd = open(path.c_str(), O_RDONLY);
        if(fd < 0){
          Log(Error, "file open fail");
          return;
        }
        else{
          ssize_t filesize = req.GetFileSize();
          sendfile(sock, fd, nullptr, filesize);
        }
        close(fd);
      }
      void EchoCgiExe(){
        //通过子进程来处理参数，子进程调用exec函数，用其它进程替换处理
        //环境变量有全局属性，进程替换不会被替换，文件描述符进程替换也不会被替换，进程替换只是替换数据和代码
        //GET参数通过环境变量传递
        //POST参数通过管道给子进程给替换进程
        //请求方法通过环境变量传递。子进程要获取参数，不同请求方法，传参方式不同
        
        //需要将content_length传入，读去字符，不知道读取多少或阻塞
        std::string content_length = std::to_string(req.GetContenLength());
        std::string content_length_env = "CONTENT_LENGTH_ENV= ";
        content_length_env += content_length;
        putenv((char *)content_length_env.c_str());
        std::string method;
        std::string method_env = "METHOD_ENV=";
        std::string parameter;
        std::string parameter_env = "PARAMETER_ENV=";
        std::string body;
        std::string path;
        int pipe_in[2];
        int pipe_out[2];
        //in out相对于子进程
        pipe(pipe_in);
        pipe(pipe_out);
        
        method = req.GetMethod();
        method_env += method;
        putenv((char *)method_env.c_str());

        pid_t pid = fork();
        if(pid == 0){
          //child
          close(pipe_in[1]);
          close(pipe_out[0]);


          if(req.IsGetMethod()){
            //GET && 带参
            parameter = req.GetParameter();
            parameter_env += parameter;
            putenv((char *)parameter_env.c_str());
          }
          //替换程序，不知道管道文件描述符
          //规定从标准输入读
          //从标准输出写
          //重定向
          dup2(pipe_in[0], 0);
          dup2(pipe_out[1], 1);
          path = req.GetPath();
          //exec
          execl(path.c_str(), path.c_str(), nullptr);

          exit(0);
        }
        //father
        close(pipe_in[0]);
        close(pipe_out[1]);

        if(req.IsPostMethod()){
          //POST,有正文
          body = req.GetBody();
          std::cout<<req.GetContenLength()<<std::endl;
          Log(Notice, body);
          //write(pipe_in[1], body.c_str(), body.size());
          for(size_t i =0; i < body.size(); i++){
            write(pipe_in[1], &body[i], 1);
          }
        }
        //从管道读子进程返回结果
        ssize_t s = 0;
        char c;
        std::string tmp;
        //这里可以read字符，判断是否读到0
        //替换进程会关闭
        do{
          s = read(pipe_out[0], &c, 1);
          tmp+=c;
          if(s > 0){
            //将结果发送进网络
            send(sock, &c, 1, 0);
          }

        }while(s > 0);
        close(pipe_out[0]);


        waitpid(pid, nullptr, 0);
      }


  public:
    //不能缺省
    EndPoint(int _sock)
    :sock(_sock)
    {}
    void RecvRequest(){
      //获得请求行
      GetRequestLine();
      //获得报头&&空行
      GetRequestHeader();
      //获得正文
      if(req.IsGetRequestBody()){
        GetRequestBody();
      }
      //获取并且细分完请求
    }
    void MakeResponse(){
      //分析请求,只响应GET和POST请求
      //将path与参数分离
      //GET && 有参数    路径 path 参数 parameter
      //Get  无参数      路径 path 
      //POST             路径 path  参数 body
      int code = 200;//状态码
      std::string path;
      size_t filesize = 0;
      if(!req.IsLegalMethod()){
        //其它方法
        code = 404;
        Log(Warning, "method is ilegal");
        goto end;
      }
      if(req.IsGetMethod()){
        //GET
        req.ParseRequestUrl();
      }
      else{
        //POST方式
        req.UrlSetToPath();
      }
      //说明是访问一个目录里的index.html
      req.AddHomePage();
      path = req.GetPath();
      Log(Notice, path);


      //资源判断，目录，可执行程序，普通网页
      struct stat st;
      if(stat(path.c_str(), &st) < 0){
        //文件不存在
        code = 404;
        Log(Warning, "file is not exit");
        goto end;
      }

      //文件存在
      if(S_ISDIR(st.st_mode)){
        //是目录
        path += '/';
        req.SetPath(path);
        req.AddHomePage();
      } 
      else{
        //可执行程序，普通文件
        if(st.st_mode & S_IXUSR ||\
            st.st_mode & S_IXGRP ||\
            st.st_mode & S_IXOTH){
          //可执行程序 CGI技术
          req.SetCgi();
        }
        else{
          //普通网页

        }
        
      }
      if(!req.IsCgi()){
        filesize = st.st_size;
        req.SetFileSize(filesize);
      }
      


end:
      //自作响应行
      MakeResponseLine(code);
      //制作响应报头
      MakeReponseHeader();

    }
    //发送请求
    void SendResponse(){
      //发送响应行
      std::string rsp_line = rsp.GetResponseLine();
      Log(Notice, rsp_line);
      send(sock, rsp_line.c_str(), rsp_line.size(), 0);
      //发送响应报头和空行
      auto rsp_header = rsp.GetResponseHeader();
      for(auto& line : rsp_header){
        Log(Notic, line);
        send(sock, line.c_str(), line.size(), 0);
      }

      //发送响应正文
      if(req.IsCgi()){
        //CGI
        EchoCgiExe();

      }
      else{
        //不是Cgi处理方式
        std::string path = req.GetPath();
        EchoNoCgiExe(path);
      }
      
    }
    //短连接
    ~EndPoint(){
      if(sock > 0){
        close(sock);
      }
    }


};

class Entry{
  public:
    static void handlerhttp(int sock){
      //int sock = *(int *)argc;
      //释放申请的空间
      //delete (int *)argc;
//条件编译     
#ifdef debug 
      char buf[10240];
      size_t s = recv(sock, buf, sizeof(buf), 0);
      std::cout<<buf<<std::endl;
      close(sock);
#else
      std::cout<<"enter handlerhttp"<<std::endl;
      EndPoint *ep = new EndPoint(sock);
      ep->RecvRequest();
      ep->MakeResponse();
      ep->SendResponse();
      std::cout<<"Out handlerhttp"<<std::endl;
      delete ep;
      
#endif

    }
};
