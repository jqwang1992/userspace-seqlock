# userspace-seqlock
　　These files are about implementation of seqlock in userspace.　The most important files about seqlock are include/barrier.h and include/seq.h
  
##preparation
　　If you really want to know about what I do, you should learn what is seqlock and what is memeory barrier firstly.
About seqlock, you can see <a href="https://en.wikipedia.org/wiki/Seqlock">https://en.wikipedia.org/wiki/Seqlock</a>. About memory barrier, there is a paper that is very much to recommend, its name is Memory Barriers:  a Hardware View for Software Hackers and you can get it from <a herf="chrome-extension://oemmndcbldboiebfnladdacbdfmadadm/http://www.rdrop.com/users/paulmck/scalability/paper/whymb.2010.07.23a.pdf">chrome-extension://oemmndcbldboiebfnladdacbdfmadadm/http://www.rdrop.com/users/paulmck/scalability/paper/whymb.2010.07.23a.pdf</a>.
