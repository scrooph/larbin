// Larbin
// Sebastien Ailleret
// 15-11-99 -> 12-12-01

#ifndef FETCHOPEN_H
#define FETCHOPEN_H

class ftOpen{
public:
/* Opens sockets
 * this function perform dns calls, using adns
 */
static void fetchDns ();

/* Opens sockets
 * Never block (only opens sockets on allready known sites)
 * work inside the main thread
 */
static void fetchOpen ();

};

#endif // FETCHOPEN_H
