#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

int main (int argc, char * argv []){

	if (argc != 4){
		printf("Usage: <./sshP host user password>\n");
		exit(1);
	}

	//Variables declarations
	ssh_session my_ssh_session;
	ssh_channel channel;
	char buffer [1024];
	int response, nbytes;
	
	//Open a session
	my_ssh_session = ssh_new();

	if(my_ssh_session == NULL){
		printf("Error creating a new session\n");
		exit(1);
	}

	//Options for this session
	ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, argv[1]);
	
	//connect to server
	if(ssh_connect(my_ssh_session) != SSH_OK){
		fprintf(stderr, "Error connecting to server: %s\n", ssh_get_error(my_ssh_session));
		exit(1);
	}

	response = ssh_userauth_password(my_ssh_session, argv[2], argv[3]);
	if(response != SSH_AUTH_SUCCESS){
		fprintf(stderr, "Error authenticating with user and passwords: %s\n",
		ssh_get_error(my_ssh_session));
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		exit(1);
	}

	channel = ssh_channel_new(my_ssh_session);
	if (channel == NULL)
		return SSH_ERROR;
	
	response = ssh_channel_open_session(channel);
	if (response != SSH_OK)
	{
		ssh_channel_free(channel);
		return response;
	}
	
	response = ssh_channel_request_exec(channel, "ps aux");
	if (response != SSH_OK)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		return response;
	}
	
	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0)
	{
		if (write(1, buffer, nbytes) != (unsigned int) nbytes)
		{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		return SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}
	
	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		return SSH_ERROR;
	}
	
	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
	ssh_disconnect(my_ssh_session);
	printf("Desconectamos el ssh\n");
	ssh_free(my_ssh_session);
	printf("Liberamos la sesion\n");
	
  	return SSH_OK;
	return 0;
}
