#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

class reproductor
{
public:

    reproductor();
    void play(char *name);
    void stop();
    void stream(char *name,char *ip, int port);
    void pause();
    void unpause();
};

#endif // REPRODUCTOR_H
