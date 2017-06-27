#include <czmq.h>

int main (void)
{
    // Createss server keypair and saves it to .curve/keypair
    /* zcert_t *client_cert = zcert_new (); */
    /* zsys_dir_create (".curve"); */
    /* zcert_set_meta (client_cert, "name", "Client test certificate"); */
    /* zcert_save (client_cert, ".curve/keypair"); */

    zcert_t *client_cert = zcert_new ();
    zcert_t *server_cert = zcert_load(".curve/keypair");
    const char *server_key = zcert_public_txt (server_cert);

    //  Create and bind server socket
    zsock_t *server = zsock_new (ZMQ_PUSH);
    zcert_apply (server_cert, server);
    zsock_set_curve_server (server, 1);
    zsock_bind (server, "tcp://*:9000");

    //  Create and connect client socket
    zsock_t *client = zsock_new (ZMQ_PULL);
    zcert_apply (client_cert, client);
    zsock_set_curve_serverkey (client, server_key);
    zsock_connect (client, "tcp://127.0.0.1:9000");
    
    //  Send a single message from server to client
    zstr_send (server, "Hello");
    char *message = zstr_recv (client);
    assert (streq (message, "Hello"));
    free (message);
    puts ("Hello test OK");

    zcert_destroy (&client_cert);
    zcert_destroy (&server_cert);
    zsock_destroy (&client);
    zsock_destroy (&server);

    return 0;
}

