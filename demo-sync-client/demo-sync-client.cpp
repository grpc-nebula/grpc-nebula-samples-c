/*
 * Copyright 2019 Orient Securities Co., Ltd.
 * Copyright 2019 BoCloud Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/thd_id.h>
#include <grpc/support/alloc.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

static int run_times = 10000; // call times
void *multiple(void* arg);

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterClient {
public:
	GreeterClient(std::shared_ptr<Channel> channel)
		: stub_(Greeter::NewStub(channel)) {}

	// Assembles the client's payload, sends it and presents the response back
	// from the server.
	std::string SayHello(const std::string& user) {
		// Data we are sending to the server.
		HelloRequest request;
		request.set_name(user);

		// Container for the data we expect from the server.
		HelloReply reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
		Status status = stub_->SayHello(&context, request, &reply);

		// Act upon its status.
		if (status.ok()) {
			return reply.message();
		}
		else {
			std::cout << status.error_code() << ": " << status.error_message()
				<< std::endl;
			return "RPC failed";
		}
	}

private:
	std::unique_ptr<Greeter::Stub> stub_;
};


void *multiple(void * arg)
{
#if 0
	GreeterClient greeter(grpc::CreateChannel(
		"localhost:50051", grpc::InsecureChannelCredentials()));
#else
	GreeterClient greeter(grpc::CreateChannel(
		"zookeeper:///helloworld.Greeter", grpc::InsecureChannelCredentials()));
#endif
	
	
// for consistent hash testing	
#if 0
	int count = 300;
	char *name = new char[20];
	memset(name, 0, sizeof(char) * 20);
	while (count--) {
		std::string user("world");
		char str[10];
		if (!(count % 10)) {
			sprintf(str, "%0d", count);
			user.append(str);
			strcpy(name, user.c_str());
		}
		std::string reply = greeter.SayHello(name);
		std::cout << "Greeter received: " << reply << std::endl;
	}
#else
	int count = 10000;
	while (count--) {
		std::string user("world000");
		char str[10];
		sprintf(str, "%0d", count);
		user.append(str);

		std::string reply = greeter.SayHello(user);
		std::cout << "Greeter received: " << reply << std::endl;
		//Sleep(1000);
	}
#endif

}
int main(int argc, char** argv) {
	// Instantiate the client. It requires a channel, out of which the actual RPCs
	// are created. This channel models a connection to an endpoint (in this case,
	// localhost at port 50051). We indicate that the channel isn't authenticated
	// (use of InsecureChannelCredentials()).
	//gpr_set_log_verbosity(GPR_LOG_SEVERITY_INFO);
	//gpr_set_log_verbosity(GPR_LOG_SEVERITY_ERROR);
	//gpr_set_log_verbosity(GPR_LOG_SEVERITY_DEBUG);

	if (argc >= 2)
	{
          run_times = atoi(argv[1]);
	}

//pthread_t *ids = new pthread_t(3); 
	pthread_t p1, p2;
	
	
	pthread_create(&p1, NULL, multiple, NULL);
	pthread_create(&p2, NULL, multiple, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	return 0;
}
