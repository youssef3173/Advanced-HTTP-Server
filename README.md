# Advanced HTTP Server  

The internet server allows any internet client to come and read internet pages located on the computer where the server (your application is launched). 

Here is the list of the functionalities of the internet server:
- Processing of file reading requests,
- Processing of file reading requests (scripts),
- Processing of requests to read the contents of a directory (listing the contents of the files and directories contained at an address)
directories contained at an address),
- Error processing (page not present, no access rights, etc.),
- Memorization of usage statics,
- Generation of an information page on the fly about the identity of the server (name of the designers, date
and time of compilation, etc.),
- Generation of an on-the-fly information page on the server's statistics (how many requests
received, processed, etc.),
- A cache manager for recently accessed files: to reduce energy consumption,
disk accesses, calculations to be carried out while increasing the throughput of the server, 
we store in RAM memory the recently accessed pages in order to avoid having to re-read them on the hard disk or the network
on the hard disk or the network at each request.
