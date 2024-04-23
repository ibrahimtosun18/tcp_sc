# Advanced TCP Server-Client Setup with SSL/TLS

This guide outlines the setup for a TCP server-client communication system using SSL/TLS for secure connections. Follow these instructions to run, test, and secure your TCP network applications.

## Getting Started

### Basic Setup

1. **Start the Server:** Open a terminal and run the server application.
2. **Start the Client:** In a separate terminal, run the client application. Ensure the client is configured to use SSL/TLS. The client will send packages securely to the server.

### Multi-Threading Server-Client

1. **Start the Multi-Threading Server:** Run `multi_tcp_server` in one terminal. This server is capable of handling multiple SSL/TLS connections simultaneously.
2. **Start Multiple Clients:** You can start up to 10 `multi_tcp_client` instances. Name your clients while starting them to send messages simultaneously and securely to the server.

## Advanced Testing with iperf

Enhance testing capabilities by replacing the server or client with iperf, which allows for efficient performance testing and benchmarking.

- **Download iperf:** [Download iperf here](https://iperf.fr/)

## Monitoring Network Traffic

Use network monitoring tools such as Wireshark for a detailed analysis of the package traffic between the client and server, especially to inspect SSL/TLS handshakes and encrypted traffic.

- **Wireshark:** Monitor packet flow and troubleshoot network traffic effectively with Wireshark.

## Security and Error Handling

The server includes detailed error logging for SSL/TLS processes to help identify and mitigate issues such as wrong version numbers or unauthorized access attempts.

- **Error Handling:** Server logs include SSL error codes and descriptions, helping diagnose connection or security issues in real time.

## Additional Notes

Ensure that all certificates and keys are correctly configured both on the server and client sides. Store these files in secure locations and reference them accurately in your server and client applications.

By following these steps, you can set up a robust and secure TCP server-client system with SSL/TLS encryption, capable of handling multiple clients and performing advanced network testing.
