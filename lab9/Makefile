server: ftpServer.o file_utils.o
	gcc ftpServer.o file_utils.o -o server -g

client: ftpClient.o file_utils.o
	gcc ftpClient.o file_utils.o -o client -g

file_utils.o: file_utils.c
	gcc file_utils.c -c -g

ftpServer.o ftpServer.c:
	gcc ftpServer.c -c -g

ftpClient.o ftpClient.c:
	gcc ftpClient.c -c -g

clean:
	rm -rf server client *.o
