#include <unistd.h>
int main(int argc, char* argv[])
{
	int i = 0;
	while(i < 100) {
		usleep(300);
	}
	return 0;
}
