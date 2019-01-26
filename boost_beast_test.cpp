//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, small
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace my_program_state
{
    std::size_t
    request_count()
    {
        static std::size_t count = 0;
        return ++count;
    }

    std::time_t
    now()
    {
        return std::time(0);
    }
}

template<class Body>
void handler(boost::beast::http::request<Body>&& req, http::response<http::dynamic_body>& response_)
{

}

template<>
void handler(boost::beast::http::request<http::string_body>&& req, http::response<http::dynamic_body>& response_)
{

}

template<>
void handler(boost::beast::http::request<http::dynamic_body>&& req, http::response<http::dynamic_body>& response_)
{
    
    int length = req.body().size();
    
    std::string output_body_str("");
    output_body_str =boost::beast::buffers_to_string(req.body().data())  ;
    std::cout << "output body:" << output_body_str << std::endl;
    std::cout << "req.target:" << req.target() << std::endl;
    //std::cout << "req.target:" <<  << std::endl;
    std::cout << "dynamic_body" << std::endl;
    std::cout << length << std::endl;

    if (req.target() == "/count")
    {
        response_.set(http::field::content_type, "text/html");
        boost::beast::ostream(response_.body())
            << "<html>\n"
            << "<head><title>Request count</title></head>\n"
            << "<body>\n"
            << "<h1>Request count</h1>\n"
            << "<p>There have been "
            << my_program_state::request_count()
            << " requests so far.</p>\n"
            << "</body>\n"
            << "</html>\n";
    }

}

template<
    class SyncReadStream,
    class DynamicBuffer,
    class Handler1,
    class Handler2>
    void
    do_form_request(
        SyncReadStream& stream,
        DynamicBuffer& buffer,
        Handler1&& handler1,
        Handler2&& handler2,
        http::response<http::dynamic_body>& response_)
{
    using namespace boost::beast::http;
    // Start with an empty_body parser
    request_parser<empty_body> req0;

    // Read just the header. Otherwise, the empty_body
    // would generate an error if body octets were received.
    read_header(stream, buffer, req0);

    // Choose a body depending on the method verb
    switch (req0.get().method())
    {
    case verb::post:
    {
        std::cout << "field::content_type:" << req0.get()[field::content_type] << std::endl;
        //std::cout << "field::content_type" << std::endl;
        // If this is not a form upload then use a string_body
        if (req0.get()[field::content_type] != "application/x-www-form-urlencoded" &&
            req0.get()[field::content_type] != "multipart/form-data")
        {
            
            goto do_dynamic_body;
        }
            

        // Commit to string_body as the body type.
        // As long as there are no body octets in the parser
        // we are constructing from, no exception is thrown.
        request_parser<string_body> req{ std::move(req0) };

        // Finish reading the message
        read(stream, buffer, req);

        // Call the handler. It can take ownership
        // if desired, since we are calling release()
        handler1(req.release(), response_);
        break;
    }

do_dynamic_body:
    default:
    {
        // Commit to dynamic_body as the body type.
        // As long as there are no body octets in the parser
        // we are constructing from, no exception is thrown.
        request_parser<dynamic_body> req{ std::move(req0) };

        // Finish reading the message
        read(stream, buffer, req);

        // Call the handler. It can take ownership
        // if desired, since we are calling release()
        handler2(req.release(), response_);
        break;
    }
    }
}


class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    http_connection(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    // Initiate the asynchronous operations associated with the connection.
    void
    start()
    {
        read_request();
        check_deadline();
    }

private:
    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    boost::beast::flat_buffer buffer_{8192};
    boost::beast::multi_buffer buffer1_;
    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_{
        socket_.get_executor().context(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
    void
    read_request()
    {
        auto self = shared_from_this();

        //handler ha;
        do_form_request(socket_, buffer1_, handler<http::string_body>, handler<http::dynamic_body>,response_);
        write_response();
        http::async_read(
            socket_,
            buffer_,
            request_,
            [self](boost::beast::error_code ec,
                std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);
                if(!ec)
                    self->process_request();
            });
    }

    // Determine what needs to be done with the request message.
    void
    process_request()
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch(request_.method())
        {
        case http::verb::get:
            response_.result(http::status::ok);
            response_.set(http::field::server, "Beast");
            create_response();
            break;

        default:
            // We return responses indicating an error if
            // we do not recognize the request method.
            response_.result(http::status::bad_request);
            response_.set(http::field::content_type, "text/plain");
            boost::beast::ostream(response_.body())
                << "Invalid request-method '"
                << request_.method_string().to_string()
                << "'";
            break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void
    create_response()
    {
        if(request_.target() == "/count")
        {
            response_.set(http::field::content_type, "text/html");
            boost::beast::ostream(response_.body())
                << "<html>\n"
                <<  "<head><title>Request count</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Request count</h1>\n"
                <<  "<p>There have been "
                <<  my_program_state::request_count()
                <<  " requests so far.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/time")
        {
            response_.set(http::field::content_type, "text/html");
            boost::beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            boost::beast::ostream(response_.body()) << "File not found\r\n";
        }
    }

    // Asynchronously transmit the response message.
    void
    write_response()
    {
        auto self = shared_from_this();

        response_.set(http::field::content_length, response_.body().size());

        http::async_write(
            socket_,
            response_,
            [self](boost::beast::error_code ec, std::size_t)
            {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->deadline_.cancel();
            });
    }

    // Check whether we have spent enough time on this connection.
    void
    check_deadline()
    {
        auto self = shared_from_this();

        deadline_.async_wait(
            [self](boost::beast::error_code ec)
            {
                if(!ec)
                {
                    // Close socket to cancel any outstanding operation.
                    self->socket_.close(ec);
                }
            });
    }
};

// "Loop" forever accepting new connections.
void
http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
  acceptor.async_accept(socket,
      [&](boost::beast::error_code ec)
      {
          if(!ec)
              std::make_shared<http_connection>(std::move(socket))->start();
          http_server(acceptor, socket);
      });
}

int
main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if(argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80\n";
            return EXIT_FAILURE;
        }

        auto const address = boost::asio::ip::make_address(argv[1]);
        unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

        boost::asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};
        http_server(acceptor, socket);

        ioc.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
