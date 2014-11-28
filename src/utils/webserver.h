// Larbin
// Sebastien Ailleret
// 07-03-00 -> 17-03-02

#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifndef NOWEBSERVER

class Webserver{
public:
	static void  startWebserver (void *none);
	static void writeHeader(int fds);
	static void writeFooter(int fds);
	static void writeTime (int fds);
	static char *readRequest (int fds);
	static void manageAns (int fds, char *req);
};
#endif // NOWEBSERVER

#endif // WEBSERVER_H
