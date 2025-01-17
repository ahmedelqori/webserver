/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <aes-sarg@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:28:36 by aes-sarg          #+#    #+#             */
/*   Updated: 2025/01/19 18:18:53 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "MyType.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Request.hpp"
#include "ServerUtils.hpp"
#include "HttpParser.hpp"
#include "ConfigParser.hpp"

class ChunkedUploadState
{
public:
    string partial_request;
    bool headers_parsed;
    size_t content_remaining;
    string upload_path;
    size_t total_size;
    ofstream output_file;

    ChunkedUploadState()
        : headers_parsed(false), content_remaining(0), output_file(), total_size(0)
    {
    }

    ~ChunkedUploadState()
    {
        if (output_file.is_open())
        {
            output_file.close();
        }
    }

    ChunkedUploadState(const ChunkedUploadState &other)
        : partial_request(other.partial_request),
          headers_parsed(other.headers_parsed),
          content_remaining(other.content_remaining),
          upload_path(other.upload_path)
    {
        if (other.output_file.is_open())
        {
            output_file.open(upload_path.c_str(), ios::binary | ios::app);
        }
    }

    ChunkedUploadState &operator=(const ChunkedUploadState &other)
    {
        if (this != &other)
        {
            partial_request = other.partial_request;
            headers_parsed = other.headers_parsed;
            content_remaining = other.content_remaining;
            upload_path = other.upload_path;

            if (output_file.is_open())
                output_file.close();
            if (other.output_file.is_open())
                output_file.open(upload_path.c_str(), ios::binary | ios::app);
        }
        return *this;
    }

    ofstream &getOutputFileStream()
    {
        if (!output_file.is_open())
            output_file.open(upload_path.c_str(), ios::binary | ios::app);
        return output_file;
    }
};

class RequestHandler
{
private:
    map<int, ChunkedUploadState> chunked_uploads;
    map<int, ResponseInfos> responses_info;

public:
    RequestHandler();
    Request request;

    ConfigParser server_config;

    void handleRequest(int client_sockfd, string req, int epoll_fd);
    bool isNewClient(int client_sockfd);
    ResponseInfos processRequest(const Request &request);
    ResponseInfos handleGet(const Request &request);
    ResponseInfos handlePost(const Request &request);
    ResponseInfos processUpload(Request request, string uploadPath);
    void cleanupConnection(int epoll_fd, int fd);
    void processPostData(int client_sockfd, const string &data, int epoll_fd);
    ResponseInfos uploadFile(Request request);
    void processChunkedData(int client_sockfd, const string &data, int epoll_fd);
    ResponseInfos handleDelete(const Request &request);
    void modifyEpollEvent(int epoll_fd, int fd, uint32_t events);
    void handleWriteEvent(int epoll_fd, int current_fd);
    ResponseInfos serveRessourceOrFail(RessourceInfo ressource);
    bool matchLocation(LocationConfig &loc, const string url, const Request &request);
};

ostream &operator<<(ostream &os, const Request &request);
