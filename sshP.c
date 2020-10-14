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
	int response;
	
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
	

	return 0;
}
