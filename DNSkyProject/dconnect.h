
extern bool ValidIP;
extern bool hooked;

extern int (WINAPI *dconnect)(SOCKET, const struct sockaddr*, int);
extern int WINAPI myconnect(SOCKET s, const struct sockaddr *name, int namelen);