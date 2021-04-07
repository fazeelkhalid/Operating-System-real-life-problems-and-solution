  #include <unistd.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include<iostream>
  using namespace std;

  int main()
  {
      int fd1[2];
      pipe(fd1);
      int cpid=fork();
      if(cpid==0)
      {
      //child is write process
      
        close(fd1[0]);
        char message[] ="hello parent \n";
        int w= write(fd1[1],message, strlen(message));
        _exit(0);
        
      }
      else if(cpid>0)
      {
      //parent is read process
         cout<<"message sent from child process"<<endl;
         wait(NULL);
         close(fd1[1]);
         char * message =new char[20];
         int r=read(fd1[0],message,20);
         write(1,message,r);
       _exit(0);
  
      }
   
      // while(true);
       return 0;
        
       
    }

