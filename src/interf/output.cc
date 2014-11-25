// Larbin
// Sebastien Ailleret
// 03-02-00 -> 10-12-01

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "options.h"

#include "types.h"
#include "global.h"
#include "fetch/file.h"
#include "utils/text.h"
#include "utils/debug.h"
#include "interf/useroutput.h"
#include "interf/output.h"
#include "utils/mypthread.h"


/** The fetch failed
 * @param u the URL of the doc
 * @param reason reason of the fail
 */
void Output::fetchFail (url *u, FetchError err, bool interesting) {
	UserOutput uop = UserOutput();
#ifdef SPECIFICSEARCH
  if (interesting
      || (privilegedExts[0] != NULL && matchPrivExt(u->getFile()))) {
	  uop.failure(u, err);
  }
#else // not a SPECIFICSEARCH
  uop.failure(u, err);
#endif
}

/** It's over with this file
 * report the situation ! (and make some stats)
 */
void Output::endOfLoad (html *parser, FetchError err) {
	UserOutput uop = UserOutput();
  answers(err);
  switch (err) {
  case success:
#ifdef SPECIFICSEARCH
    if (parser->isInteresting) {
      interestingPage();
      loaded(parser);
    }
#else // not a SPECIFICSEARCH
    uop.loaded(parser);
#endif // SPECIFICSEARCH
    break;
  default:
    fetchFail(parser->getUrl(), err, parser->isInteresting);
    break;
  }
}

#ifdef THREAD_OUTPUT
/** In this thread, end user manage the result of the crawl
 */
static void *startOutput (void *none) {
  UserOutput uop = UserOutput();
  uop.initUserOutput();
  for (;;) {
    Connexion *conn = global::userConns->get();
    endOfLoad((html *)conn->parser, conn->err);
    conn->recycle();
    global::freeConns->put(conn);
  }
  return NULL;
}

void Output::initOutput () {
  startThread(startOutput, NULL);
}

#else // THREAD_OUTPUT not defined

void Output::initOutput () {
	UserOutput uop = UserOutput();
	uop.initUserOutput();
}

#endif // THREAD_OUTPUT
