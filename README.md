# OptimalPathFinderClient

The Client for the server [OptimalPathFinder](https://github.com/PaThreek/OptimalPathFinder) application. Generates the image of the path from point A to point B based on the response from the Server.

The application tries to connect to the Server application [OptimalPathFinder](https://github.com/PaThreek/OptimalPathFinder) via socket connection. After the connection is estabilished, the user is prompted to input the starting and ending coordinates. Those coordinates are then transferred to the server. The server replies with the path from starting to ending point, if are the given coordinates valid and reachable.

The application supports the following commands:

P x1 y1 x2 y2: Request path from a Server for an optimal path from point x1, y1 to point x2, y2 and generate the image.
 
Q: Quit the program.

Developed, compiled and tested under Ubuntu 22.04 LTS. Compiler gcc 11.3.0.

Ideas to improve:
* the socket connection could be improved with more complex error handling, timeouts, reconnections, etc.
* the data transmission over the socket connection could be improved with extended error handling
* reading and writing of the data received/sent from/to the Server is currently hard-coded to one purpose, that is the path request. There can be defined a more generic way how to deal with the data received/sent from the server
