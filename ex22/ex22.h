#ifndef _ex22_h
#define _ex22_h

// This "extern" makes THE_SIZE available to other .c files:
extern int THE_SIZE;

// These get and set an internal static variable in ex22.c:
int get_age();
void set_age(int age);

// This updates a static variable that's inside update_ratio:
double update_ratio(double ratio);

void print_size();

#endif //_ex22_h
