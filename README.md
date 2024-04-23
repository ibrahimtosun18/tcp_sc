# Basic TCP Server-Client Setup

This guide outlines the basic setup for a tcp server-client communication system. Follow these instructions to run and test your tcp network applications.

## Getting Started

1. **Start the Server:** Open a terminal and run the server application.
2. **Start the Client:** In a separate terminal, run the client application. The client will send packages to the server.
3. **Start multiple multiple threading server-client** Start the multi_tcp_server on one terminal. After that, you can start up to 10 multi_tcp_clients. Name your clients while starting them. They can send messages simultaneously to server

## Advanced Testing with iperf

You can replace the server or client with iperf to enhance testing capabilities. iperf allows for more efficient performance testing and benchmarking.

- **Download iperf:** [Download iperf here](https://iperf.fr/)

## Monitoring Network Traffic

For detailed analysis of the package traffic between the client and server, use network monitoring tools such as Wireshark.

- **Wireshark:** Use Wireshark to monitor the packet flow and troubleshoot the network traffic effectively.

By following these steps, you can set up and monitor a basic tcp server-client system, and perform advanced network testing.
