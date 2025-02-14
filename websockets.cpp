#include <libwebsockets.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstring>

// Shared queue for messages
std::queue<std::string> message_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;
bool stop_consumer = false;

// WebSocket connection
struct lws *global_wsi = nullptr;

// Producer threads: Generate log messages
void producer(int id) {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::string message = "Log from Producer " + std::to_string(id) + ": Message " + std::to_string(i);

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            message_queue.push(message);
        }
        queue_cv.notify_one(); // Notify the consumer thread
    }
}

// Consumer thread: Reads from the queue and sends messages via WebSocket
void consumer() {
    while (!stop_consumer) {
        std::unique_lock<std::mutex> lock(queue_mutex);

        queue_cv.wait(lock, [] { return !message_queue.empty() || stop_consumer; });

        while (!message_queue.empty()) {
            std::string message = message_queue.front();
            message_queue.pop();
            lock.unlock();

            // Send the message over WebSocket
            if (global_wsi) {
                unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 1024 + LWS_SEND_BUFFER_POST_PADDING];
                std::strcpy((char *)&buf[LWS_SEND_BUFFER_PRE_PADDING], message.c_str());
                lws_write(global_wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], message.size(), LWS_WRITE_TEXT);
                std::cout << "Sent: " << message << "\n";
            }

            lock.lock();
        }
    }
}

// WebSocket callback
static int callback_websocket(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              void *user,
                              void *in,
                              size_t len) {
    switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        std::cout << "WebSocket connection established.\n";
        global_wsi = wsi;
        break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
        std::cout << "Received message: " << std::string((char *)in, len) << "\n";
        break;

    case LWS_CALLBACK_CLIENT_CLOSED:
        std::cout << "WebSocket connection closed.\n";
        global_wsi = nullptr;
        stop_consumer = true;
        queue_cv.notify_all(); // Notify the consumer thread to stop
        break;

    default:
        break;
    }
    return 0;
}

// Main function
int main() {
    // Set up WebSocket protocol
    static struct lws_protocols protocols[] = {
        {"example-protocol", callback_websocket, 0, 1024},
        {nullptr, nullptr, 0, 0} // Terminator
    };

    // Create WebSocket context
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        std::cerr << "Failed to create WebSocket context.\n";
        return -1;
    }

    // Connect to WebSocket server
    struct lws_client_connect_info ccinfo = {};
    ccinfo.context = context;
    ccinfo.address = "echo.websocket.org"; // WebSocket server address
    ccinfo.port = 443;
    ccinfo.path = "/";
    ccinfo.ssl_connection = LCCSCF_USE_SSL;
    ccinfo.protocol = protocols[0].name;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (!wsi) {
        std::cerr << "Failed to connect to WebSocket server.\n";
        lws_context_destroy(context);
        return -1;
    }

    // Start producer threads
    std::thread producer1(producer, 1);
    std::thread producer2(producer, 2);

    // Start the consumer thread
    std::thread consumer_thread(consumer);

    // Run the WebSocket event loop
    while (!stop_consumer) {
        lws_service(context, 1000);
    }

    // Wait for threads to finish
    producer1.join();
    producer2.join();
    consumer_thread.join();

    // Clean up
    lws_context_destroy(context);
    return 0;
}
