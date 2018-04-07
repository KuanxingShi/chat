/*
 * work.h
 *
 *  Created on: 2013年12月17日
 *      Author: zhujy
 */

#ifndef WORK_H_
#define WORK_H_

#define CLIENTCOUNT 256
#define BODYBUF 1024

#include "myoracle.h"

struct msg_t
{
	unsigned char head[4];
	char body[BODYBUF];
};

class work
{
public:
	work(int port);
	~work();
	void run();	
	int conn2db(const char *Hostname, const char *User, const char *Password,
			const char *DBName);
	
private:
	void fix_socket_client(int index, int st); //将accept的客户端连接安装到socket_client[10]的数组中
	int auth_passwd(int userid, const char *passwd);
	void broadcast_user_status(); //向socket_client[]数组中所有client广播用户状态消息
	void sendmsg(const struct msg_t *msg, ssize_t msglen); //处理send消息
	void loginmsg(int st, int o_userid, const char *passwd); //处理login消息
	void user_logout(int st); //client socket连接断开
	int socket_recv(int st);
	int socket_accept();
	int setnonblocking(int st);//将socket设置为非阻塞

	int listen_st;
	myoracle *db;
	int socket_client[CLIENTCOUNT]; //申明socket_client数组，管理client的socket连接
};
#endif /* WORK_H_ */
