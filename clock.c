#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define START_CHAR      'a'
// -q flag means `quiet`
// redirect stderr to /dev/null
// run in background to not block
#define PLAY_CMD        "play %c.aif -q 2> /dev/null &"

void play(int);

volatile int verbose;

int main(int argc, char** argv)
{
    int i, hour, cap, current, remainder;
    verbose = 0;
    hour = 0;
    cap = 1;
    current = 1;

    for (i = 1; i < argc; i++)
    {
        if (strcmp("-v", argv[i]) == 0)
        {
            verbose = 1;
        }
        else
        {
            hour = atoi(argv[i]);
        }
    }

    if (argc < 2 || hour <= 0 || hour > 24)
    {
        printf("USAGE: ./clock [hour (1-24)] [options]\n");
        return 1;
    }


    while ((cap + current + 1) <= hour)
    {
        current++;
        cap += current;
    }

    remainder = hour - cap;

    for (i = current; i > 0; i--)
    {
        play(i);

        while (remainder >= i)
        {
            remainder -= i;
            play(i);
        }
    }

    return 0;
}

// level is between 1 and 4
void play(int level)
{
    char buf[100];
    char note = START_CHAR + level - 1;

    sprintf(buf, PLAY_CMD, note);
    system(buf);

    if (verbose)
        printf("Playing a(n) %c (value %d)\n", note, level);
    
    sleep(1);

}
