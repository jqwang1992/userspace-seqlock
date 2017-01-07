all:seqlock
flag = -lpthread
seqlock:seqlock.c
	gcc $^ -o $@  $(flag)
.PHONY:clean
clean:
	rm seqlock
