#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

void run_single_client(int client_id, const std::string& host, int port, const std::string& password) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "[Client " << client_id << "] Failed to create socket" << std::endl;
        return;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "[Client " << client_id << "] Connection failed" << std::endl;
        close(sockfd);
        return;
    }

    std::cout << "[Client " << client_id << "] Connected!" << std::endl;

    // Send IRC commands - all join #room
    std::string nickname = "Bot" + std::to_string(client_id);
    std::string username = "user" + std::to_string(client_id);
    
    std::vector<std::string> commands = {
        "PASS " + password,
        "NICK " + nickname,
        "USER " + username + " 0 * :" + username,
        "JOIN #room",
        "PRIVMSG #room :Hello #room! I'm client " + std::to_string(client_id),
        "PRIVMSG #room :Stress testing the #room channel!",
        "PRIVMSG #room :This is bot number " + std::to_string(client_id) + " in #room"
    };

    // Send basic commands first
    for (size_t i = 0; i < 4; i++) {  // PASS, NICK, USER, JOIN
        std::string full_cmd = commands[i] + "\r\n";
        send(sockfd, full_cmd.c_str(), full_cmd.length(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // Wait a bit for registration and join to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Send messages to #room
    for (size_t i = 4; i < commands.size(); i++) {
        std::string full_cmd = commands[i] + "\r\n";
        send(sockfd, full_cmd.c_str(), full_cmd.length(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    // Additional random activity in #room
    int extra_messages = client_id % 3 + 1;  // 1-3 extra messages
    for (int i = 0; i < extra_messages; i++) {
        std::string msg = "PRIVMSG #room :Extra message " + std::to_string(i+1) + 
                         " from bot " + std::to_string(client_id) + " in #room\r\n";
        send(sockfd, msg.c_str(), msg.length(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Leave #room properly
    std::string part_cmd = "PART #room :Bot " + std::to_string(client_id) + " leaving #room. Test complete!\r\n";
    send(sockfd, part_cmd.c_str(), part_cmd.length(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Quit
    std::string quit_cmd = "QUIT :Stress test finished for bot " + std::to_string(client_id) + "\r\n";
    send(sockfd, quit_cmd.c_str(), quit_cmd.length(), 0);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    close(sockfd);
    std::cout << "[Client " << client_id << "] Disconnected" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: " << argv[0] << " <host> <port> <password> <num_clients>" << std::endl;
        return 1;
    }

    std::string host = argv[1];
    int port = std::atoi(argv[2]);
    std::string password = argv[3];
    int num_clients = std::atoi(argv[4]);

    std::cout << "Starting " << num_clients << " clients..." << std::endl;

    std::vector<std::thread> threads;
    
    // Launch clients in batches
    int batch_size = 50;  // Adjust based on your system
    
    for (int i = 1; i <= num_clients; i++) {
        threads.emplace_back(run_single_client, i, host, port, password);
        
        // Control concurrent connections
        if (i % batch_size == 0) {
            std::cout << "Launched batch ending at client " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Small delay between launches
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Wait for all to complete
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All clients completed!" << std::endl;
    return 0;
}


