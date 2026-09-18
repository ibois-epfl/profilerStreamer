#ifndef MOCK_OPCUA_SERVER_HH
#define MOCK_OPCUA_SERVER_HH

#pragma once

// STL headers
#include <atomic>
#include <cmath>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

// open62541
#include <open62541/server.h>
#include <open62541/server_config_default.h>

namespace ProfilerStreaming::Communicate
{
    /*
     * A real open62541 OPC UA server used in tests to simulate a device streaming data
     * over the wire. Unlike MockOPCUACommunicator, this actually runs a network-reachable
     * OPC UA server so that the real OPCUACommunicator/OPCUARecorder client code can be
     * exercised end-to-end against it (via opc.tcp://127.0.0.1:<port>).
     *
     * It exposes a single Byte-array node (matching what OPCUACommunicator expects: a
     * 2-byte big-endian distance value) at the given namespace index / identifier, and
     * updates it on a repeated timer to simulate an oscillating rangefinder reading.
     */
    class MockOPCUAServer
    {
    public:
        MockOPCUAServer(uint16_t port, std::pair<int, int> namespaceIndexAndIdentifier, double updateIntervalMiliSec = 20.0)
            : nodeId(UA_NODEID_NUMERIC(static_cast<UA_UInt16>(namespaceIndexAndIdentifier.first),
                                        static_cast<UA_UInt32>(namespaceIndexAndIdentifier.second)))
        {
            // Build a fresh config rather than reconfiguring UA_Server_new()'s default
            // (which is already bound to port 4840) - see open62541's own
            // UA_Server_newForUnitTest() for the same pattern.
            UA_ServerConfig config = {};
            if (UA_ServerConfig_setMinimal(&config, port, nullptr) != UA_STATUSCODE_GOOD)
            {
                throw std::runtime_error("MockOPCUAServer: failed to configure server on requested port");
            }

            server = UA_Server_newWithConfig(&config);
            if (!server)
            {
                throw std::runtime_error("MockOPCUAServer: failed to create UA_Server");
            }

            RegisterNamespaceIfNeeded(nodeId.namespaceIndex);
            AddDistanceVariableNode();
            UA_Server_addRepeatedCallback(server, &MockOPCUAServer::UpdateDistanceValue, this, updateIntervalMiliSec, nullptr);
        }

        ~MockOPCUAServer()
        {
            Stop();
            UA_Server_delete(server);
        }

        MockOPCUAServer(const MockOPCUAServer&) = delete;
        MockOPCUAServer& operator=(const MockOPCUAServer&) = delete;

        void Start()
        {
            if (serverThread.joinable())
            {
                throw std::runtime_error("MockOPCUAServer: already running");
            }
            running.store(true, std::memory_order_release);
            serverThread = std::thread([this]() 
            {
                UA_Server_run_startup(server);
                while (running.load(std::memory_order_acquire))
                {
                    UA_Server_run_iterate(server, true);
                }
                UA_Server_run_shutdown(server);
            });
        }

        void Stop()
        {
            running.store(false, std::memory_order_release);
            if (serverThread.joinable())
            {
                serverThread.join();
            }
        }

    private:
        // A fresh server only has namespaces 0 and 1 registered. Real devices report their
        // data under a vendor-specific namespace index (e.g. 6), so pad out placeholder
        // namespaces up to that index or UA_Server_addVariableNode rejects the node.
        void RegisterNamespaceIfNeeded(UA_UInt16 desiredNamespaceIndex)
        {
            for (UA_UInt16 ns = 2; ns <= desiredNamespaceIndex; ++ns)
            {
                std::string uri = "urn:profilerStreamer:mock:namespace" + std::to_string(ns);
                UA_UInt16 addedIndex = UA_Server_addNamespace(server, uri.c_str());
                if (addedIndex != ns)
                {
                    UA_Server_delete(server);
                    throw std::runtime_error("MockOPCUAServer: failed to register namespace index " + std::to_string(ns));
                }
            }
        }

        void AddDistanceVariableNode()
        {
            UA_VariableAttributes attr = UA_VariableAttributes_default;
            UA_Byte initialValue[2] = {0, 0};
            UA_Variant_setArray(&attr.value, initialValue, 2, &UA_TYPES[UA_TYPES_BYTE]);
            attr.description = UA_LOCALIZEDTEXT(const_cast<char*>("en-US"), const_cast<char*>("mock rangefinder distance"));
            attr.displayName = UA_LOCALIZEDTEXT(const_cast<char*>("en-US"), const_cast<char*>("distance"));
            attr.dataType = UA_TYPES[UA_TYPES_BYTE].typeId;
            attr.accessLevel = UA_ACCESSLEVELMASK_READ;

            UA_QualifiedName name = UA_QUALIFIEDNAME(nodeId.namespaceIndex, const_cast<char*>("distance"));
            UA_StatusCode status = UA_Server_addVariableNode(
                server, nodeId, UA_NS0ID(OBJECTSFOLDER), UA_NS0ID(ORGANIZES),
                name, UA_NS0ID(BASEDATAVARIABLETYPE), attr, nullptr, nullptr);

            if (status != UA_STATUSCODE_GOOD)
            {
                UA_Server_delete(server);
                throw std::runtime_error("MockOPCUAServer: failed to add distance variable node");
            }
        }

        static void UpdateDistanceValue(UA_Server* server, void* data)
        {
            auto* self = static_cast<MockOPCUAServer*>(data);
            self->distanceValueMm = 1000 + static_cast<int>(500 * std::sin(0.1 * self->tick));
            ++self->tick;

            UA_Byte bytes[2] = {
                static_cast<UA_Byte>((self->distanceValueMm >> 8) & 0xFF),
                static_cast<UA_Byte>(self->distanceValueMm & 0xFF)
            };
            UA_Variant value;
            UA_Variant_init(&value);
            UA_Variant_setArray(&value, bytes, 2, &UA_TYPES[UA_TYPES_BYTE]);
            UA_Server_writeValue(server, self->nodeId, value);
        }

        UA_Server* server = nullptr;
        UA_NodeId nodeId;
        std::thread serverThread;
        std::atomic<bool> running{false};
        int distanceValueMm = 1000;
        int tick = 0;
    };
}

#endif // MOCK_OPCUA_SERVER_HH
