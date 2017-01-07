# userspace-seqlock
　　These files are about implementation of seqlock in userspace.　The most important files about seqlock are include/barrier.h and include/seq.h．　barrier.h is the implementation in userspace. seq.h is the impementation in userspace.
  
##Preparation
　　If you really want to know about what I do, you should learn what is seqlock and what is memeory barrier firstly.
About seqlock, you can see <a href="https://en.wikipedia.org/wiki/Seqlock">https://en.wikipedia.org/wiki/Seqlock</a>. About memory barrier, there is a paper that is very much to recommend, its name is Memory Barriers:  a Hardware View for Software Hackers and you can get it from <a herf="chrome-extension://oemmndcbldboiebfnladdacbdfmadadm/http://www.rdrop.com/users/paulmck/scalability/paper/whymb.2010.07.23a.pdf">http://www.rdrop.com/users/paulmck/scalability/paper/whymb.2010.07.23a.pdf</a>.

##Using
　　Using write lock, you can put your critical code between <font color="red">write_seqlock(&seqlock)</font> and <font color="blue">write_sequnlock(&seqlock)</font>.

　　Using read lock, you can do as follow:<br>
```c
  unsigned int start;
  do{
    start = read_seqbegin(&seqlock);
  }while(read_seqretry(&seqlock, start));
```
