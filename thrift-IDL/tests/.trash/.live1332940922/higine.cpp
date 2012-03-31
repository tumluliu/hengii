/*
 * =====================================================================================
 *
 *       Filename:  higine.cpp
 *
 *    Description:  The main engine implementation. This file is created based on the 
 *    				autogenerated skeleton file by thrift, which illustrates how to 
 *    				build a server. 
 *
 *        Version:  0.6
 *        Created:  03/12/2012 11:58:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */

//#include "higine.h"
//#include "jobscheduler.h"
#include "higine.h"

using namespace HPGC::HiGIS::Server;

int main(int argc, char **argv) {
	//	init_daemon();
	shared_ptr<HpgcJobHandler> handler(new HpgcJobHandler());
	shared_ptr<TProcessor> processor(new HpgcJobProcessor(handler));
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(SESSION_POOL_SIZE);
	shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();
	TNonblockingServer server(processor, protocolFactory, PORT, threadManager);
	cout << "================================================" << endl;
	cout << "higis engine v" << VERSION << " working on port " << PORT << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << "================================================" << endl;
	server.serve();
	return 0;
}