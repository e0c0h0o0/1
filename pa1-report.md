Describe any design decisions you made. These may be minimal for pa1
PA1 is just like "filling in the blanks", there's very little decisions need to be made.
There are several iterators need to be designed a little bit, like HeapPageIterator and HeapFileIterator and a SeqScanIterator;
Generally, I used lower level iterator in higher level iterator and saved some other status need to be track.
For example, I used HeapPageIterator in HeapFileIterator, and I also keep tracking the current page number when iterator is iterating.

Discuss and justify any changes you made to the API.
Nothing changed to API.

Describe any missing or incomplete elements of your code.
The description said I need to inherit DbIterator interface while I did not.
The reason is to use dynamic binding system I have to use pointer, which is not good option.
C++ uses trait instead of directly inherit an interface.

Describe how long you spent on the assignment, and whether there was anything you found particularly difficult or confusing.
I spend around 8-10 hours for this assignment. The most difficult part is handling some const constraint. 
Sometimes I have to use some non-const function in const function, that did not work. 
I have to either find other work around or change the function I used const， which may requires more function to be marked const.
And eventually  I found I can not mark a specific function const, so I have to find other way.

If you collaborate with someone else, you have to describe how you split the workload.
I have done this job alone.