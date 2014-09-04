// Larbin
// Sebastien Ailleret
// 07-12-01 -> 07-12-01

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "options.h"

#include "types.h"
#include "global.h"
#include "fetch/file.h"
#include "utils/text.h"
#include "utils/debug.h"
#include "interf/output.h"

using namespace std;

static int nbdir = -1;
static int nbfile = filesPerDir;
static char buf[maxUrlSize+30];
static char urlContent[maxUrlSize+30];
static char *fileName;
static int indexFds = -1;
static int Debug = 0;
static uint endFileName;
void ldprintf(char *fmt,...);
/** A page has been loaded successfully, save it to disk
 * @param page the page that has been fetched
 */
void loaded (html *page) {
  nbfile++;
  if (nbfile < filesPerDir) {
    int tmp = nbfile;
    int pos = endFileName;
    while (tmp != 0) {
      fileName[pos--] = '0' + (tmp % 10);
      tmp /= 10;
    }
  } else { // new dir
    // create the directory
    nbdir++; nbfile = 0;
    int pos = endFileName-7;
    int tmp = nbdir;
    while (tmp != 0) {
      fileName[pos--] = '0' + (tmp % 10);
      tmp /= 10;
    }
    fileName[endFileName-6] = 0;
    if (mkdir(fileName, S_IRWXU) != 0) perror("trouble while creating dir");
    fileName[endFileName-6] = '/';
    // open new index
    close(indexFds);
    strcpy(fileName+endFileName-5, "index");
    indexFds = creat(fileName, S_IRWXU);
    if (indexFds < 0) {
      cerr << "cannot open file " << fileName << "\n";
      exit(1);
    }
    // new filename
    fileName[endFileName-5] = 'f';
    for (uint i=endFileName; i>endFileName-5; i--) fileName[i]='0';
  }
  int fd = creat(fileName, S_IRWXU);
  
  if (fd < 0) {
    cerr << "cannot open file " << fileName << "\n";
    exit(1);
  }
  int s=0;
  int ur=0;
  s = sprintf(buf, "%4u ", nbfile);
#ifdef URL_TAGS
  s += sprintf(buf+s, "(%u) ", page->getUrl()->tag);
#endif // URL_TAGS
  s += page->getUrl()->writeUrl(buf+s);
  ur = page->getUrl()->writeUrl(urlContent);
  buf[s++] = '\n';
  ecrireBuff(indexFds, buf, s);
  ecrireBuff(fd, page->getPage(), page->getLength());
  close(fd);
#ifdef SAVE_DB
	//get file
	ldprintf("%s\n",fileName);
	//get url
	ldprintf("%s\n",urlContent);
	//write the web info into mogodb
	
	char cmdContent[4096] = {0};
	sprintf(cmdContent,"%s queue.php %s %s",PHP_CGI,fileName,urlContent);
	system(cmdContent);
	ldprintf("%s\n",cmdContent);
#endif

}

/** The fetch failed
 * @param u the URL of the doc
 * @param reason reason of the fail
 */
void failure (url *u, FetchError reason) {
  // do nothing
}

/** initialisation function
 */
void initUserOutput () {
  mkdir(saveDir, S_IRWXU);
  endFileName = strlen(saveDir);
  fileName = new char[endFileName+maxUrlSize+50];
  strcpy(fileName, saveDir);
  if (fileName[endFileName-1] != '/') fileName[endFileName++] = '/';
  strcpy(fileName+endFileName, "d00000/f00000");
  endFileName += 12; // indique le dernier char ecrit
}

/** stats, called in particular by the webserver
 * the webserver is in another thread, so be careful
 * However, if it only reads things, it is probably not useful
 * to use mutex, because incoherence in the webserver is not as critical
 * as efficiency
 */
void outputStats(int fds) {
  ecrire(fds, "Nothing to declare");
}

void ldprintf(char *fmt,...){
	va_list ap;

	if(Debug){
		va_start(ap, fmt);
		vfprintf(stdout, fmt, ap);
		va_end(ap);

		fflush(stdout);
		return;
	}else{
		return;
	}
}