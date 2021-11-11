#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "logger.h"

#define MAX_MONITOR 2048
#define LENGTH 32
#define MONITOR_SIZE (sizeof(struct inotify_event))
#define BUFFER_LENGTH MAX_MONITOR*(MONITOR_SIZE + LENGTH)

int main(int argc, char **argv){

    // Argument Checking
    if(argc != 2){
        errorf("Invalid number of arguments, Must be 1 argument");
        exit(-1);
    }
    
    // Place your magic here
    int wFidi;
    int wDNotify;
    const char *path = argv[1];
    char buffer[BUFFER_LENGTH];
    wFidi = inotify_init();

    if (wFidi < 0){
        perror("Error initializing Notify");
    }

    wDNotify = inotify_add_watch(wFidi, path, IN_CREATE|IN_MOVED_FROM|IN_MOVED_TO|IN_DELETE);

    if (wDNotify == -1){
        warnf("Error adding monitor at %s \n", path);
    }else{
        infof("Monitoring File/Directory at %s \n", path);
    }

    int i = 0;

    while(1){
        i = 0;
        int readingNum = read(wFidi, buffer, BUFFER_LENGTH);

        if (readingNum < 0){
            perror("Error reading file/directory");
        }

        while(i < readingNum){
            struct inotify_event *event = (struct inotify_event*)&buffer[i];

            if (event-> len){
                if (event-> mask & IN_CREATE){
                    if (event-> mask & IN_ISDIR){
                        infof("- [Directory - Create]: %s\n", event-> name);
                    }else{
                        infof("- [File Create]: %s\n", event-> name);
                    }

                }else if (event-> mask & IN_DELETE){
                    if (event-> mask & IN_ISDIR){
                        infof("- [Directory - Removal]: %s\n", event-> name);
                    }else{
                        infof("- [File removal]: %s\n", event-> name);
                    }

                }else if (event-> mask & IN_MOVED_FROM){
                    if (event-> mask & IN_ISDIR){
                        infof("- [Directory - Rename]: %s\n", event-> name);
                    }else{
                        infof("- [File rename]: %s\n", event-> name);
                    }
                    
                }else if (event-> mask & IN_MOVED_TO){
                    if (!(event-> mask & IN_ISDIR)){
                        infof(" -> %s\n", event->name);
                    }
                }

                i += MONITOR_SIZE + event-> len;
            }
        }
    }

    inotify_rm_watch(wFidi, wDNotify);
    close(wFidi);
    return 0;
}
