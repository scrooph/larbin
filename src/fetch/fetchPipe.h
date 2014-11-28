// Larbin
// Sebastien Ailleret
// 15-11-99 -> 22-07-01

#ifndef FETCHPIPE_H
#define FETCHPIPE_H

class fetchPipe{
public:
static void checkTimeout ();
static void checkAll ();
static void pipeRead (Connexion *conn);
static void pipeWrite (Connexion *conn);
static void endOfFile (Connexion *conn);

};

#endif // FETCH_H
