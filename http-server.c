//reference AES ENCRYPTION: https://gist.github.com/bricef/2436364

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <netinet/in.h>


#include <mcrypt.h>

int encrypt( void* buffer, int buffer_len, char* IV, char* key, int key_len){

MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc",NULL);
int blocksize = mcrypt_enc_get_block_size(td);

if( buffer_len % blocksize != 0 ){
return 1;
}

mcrypt_generic_init(td, key, key_len, IV);
mcrypt_generic(td, buffer, buffer_len);
mcrypt_generic_deinit (td);
mcrypt_module_close(td);
  
  return 0;
}

int decrypt(void* buffer,int buffer_len,char* IV,char* key,int key_len){
 
MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
int blocksize = mcrypt_enc_get_block_size(td);
 
if( buffer_len % blocksize != 0 ){
return 1;
}
  
mcrypt_generic_init(td, key, key_len, IV);
mdecrypt_generic(td, buffer, buffer_len);
mcrypt_generic_deinit (td);
mcrypt_module_close(td);

return 0;

} 


void display(char* ciphertext, int len){

 int v;
 for (v=0; v<len; v++){
    printf("%d ", ciphertext[v]);
  }

 printf("\n");

}



int main()
{
	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	char *message;


	FILE *file;
	file = fopen("index.html", "r");
	char response[2048];
	char header[2048] = "HTTP/1.1 200 OK\r\n\n";
	fgets(response,1024,file);
	strcat(header,response);

	char* plaintext = response ; 
	char* IV = "AAAAAAAAAAAAAAAA";
  	char* key = "0123456789abcdef";
  	int keysize = 32;
  	char* buffer;
  	int buffer_len = 32;

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8080 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("binding done");
	
	//Listen
	listen(socket_desc , 5);
	
	 buffer = calloc(1, buffer_len);
        strncpy(buffer, plaintext, buffer_len);

        encrypt(buffer, buffer_len,IV, key, keysize);
        printf("cipher:  ");
        display(buffer , buffer_len);
      	decrypt(buffer, buffer_len, IV, key, keysize);
     	printf("decrypt: %s\n", buffer);


	
	while(1) { 
		
		new_socket = accept(socket_desc, NULL, NULL );
		//Reply to the client
		
		//send(new_socket ,header, sizeof(header), 0);
		send(new_socket, header, sizeof(header),0);
		recv(new_socket,header, sizeof(header), 0);
		close(new_socket);
	}


	return 0;
}
	
