#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "rpcpp/client/rpcprotocolclient.h"
using namespace std;
int main()
{
    // Json::Value result;
    // Json::Value request;
    // request["name"] = "add";
    // request["type"] = "int";
    // request["para1"] = 1;
    // request["para2"] = 2;
    // Json::StreamWriterBuilder wbuilder;
    // wbuilder["indentation"] = "";

    // string result = Json::writeString(wbuilder, request);
    // cout << result << endl;


    // Json::Reader reader;
    // Json::Value response;
    // reader.parse(result,response);


    // Json::StyledWriter sw;
    // ofstream os;
	// os.open("demo.json");
	// os << sw.write(response);
	// os.close();

    // ifstream in("demo.json", ios::binary);
    // reader.parse(result,response);

    // cout<<response<<endl;
    // Json::StyledWriter sw;
    // Json::Value para;
    // ofstream os;
	// os.open("demo.json");
	// os << sw.write(para);
	// os.close();
    // rpcpp::RpcProtocolClient rpc;
    // std::string request;
    // rpc.BuildRequest("echo",para,request,false);

    // cout<<request<<endl;

    return 0;
}