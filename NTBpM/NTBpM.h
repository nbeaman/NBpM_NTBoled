#ifndef NTBpM_h
#define NTBpM_h

#include <Arduino.h>
#include <painlessMesh.h>

// Define a type for a user-provided function that handles incoming messages
typedef std::function<void(uint32_t, String)> MeshReceiveCallback;

class NTBpM {
  public:
    // Constructor: accepts SSID, password, and port to set up the mesh
    NTBpM(const String& ssid, const String& password, uint16_t port);

    // Starts the mesh network (called in setup)
    void begin();

    // Regular update call for mesh maintenance (called in loop)
    void update();

    // Send a broadcast message across the mesh
    void sendBroadcast(const String& message);

    // Allow user to set a custom function to handle received messages
    void setReceiveCallback(MeshReceiveCallback cb);
	
	// Returns the node ID of the code called from
	String getNodeId();

  private:
    // Internal callbacks for mesh events
    static void receivedCallback(uint32_t from, String &msg);       // Incoming messages
    static void newConnectionCallback(uint32_t nodeId);             // Node joins
    static void changedConnectionCallback();                        // Topology change

    // Mesh network credentials
    String _ssid;
    String _password;
    uint16_t _port;

    // Handles tasks like timed message sending
    Scheduler userScheduler;

    // Mesh object for painlessMesh functionality
    painlessMesh mesh;

    // Task object for periodic message sending
    Task taskSendMessage;

    // Stores the user's receive handler
    static MeshReceiveCallback userCallback;
};

#endif
