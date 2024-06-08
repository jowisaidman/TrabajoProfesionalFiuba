/*
* @brief Function that calls the control plane to communicate 
* a new node has joined the network
*/
void on_peer_connected();

/*
* @brief Function that calls the control plane to communicate 
* a node has left the network
*/
void on_peer_lost();

/*
* @brief Function that calls the control plane to communicate 
* that a new message has been received
*/
void on_peer_message();