# System Architecture

## Overview

EKNMusic follows a client-server architecture with a serverless backend and cross-platform desktop client.

## Components

### 1. Backend Server
- **Technology**: Python
- **Hosting**: AWS Lambda (Serverless)
- **Database**: PostgreSQL (AWS RDS)
- **Local Development**: Docker containers

### 2. Desktop Client
- **Technology**: C++ with Qt Framework
- **Platforms**: Windows, macOS, Linux
- **Communication**: REST API / WebSocket to backend

## Architecture Diagram

```
┌─────────────────────────────────────────┐
│         Desktop Clients (Qt C++)        │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐ │
│  │ Windows │  │  macOS  │  │  Linux  │ │
│  └─────────┘  └─────────┘  └─────────┘ │
└─────────────────┬───────────────────────┘
                  │
                  │ HTTPS/WSS
                  │
┌─────────────────▼───────────────────────┐
│           AWS Cloud Backend             │
│  ┌─────────────────────────────────┐   │
│  │      API Gateway / ALB          │   │
│  └───────────────┬─────────────────┘   │
│                  │                      │
│  ┌───────────────▼─────────────────┐   │
│  │    Lambda Functions (Python)    │   │
│  └───────────────┬─────────────────┘   │
│                  │                      │
│  ┌───────────────▼─────────────────┐   │
│  │   PostgreSQL (AWS RDS)          │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘

Local Development:
┌─────────────────────────────────────────┐
│      Docker Compose Environment         │
│  ┌─────────────┐    ┌──────────────┐   │
│  │   Python    │───▶│  PostgreSQL  │   │
│  │   Server    │    │   Container  │   │
│  └─────────────┘    └──────────────┘   │
└─────────────────────────────────────────┘
```

## Data Flow

1. Desktop client initiates request
2. Request routed through API Gateway
3. Lambda function processes request
4. Database operations performed on PostgreSQL
5. Response returned to client

## Scalability

- Serverless functions auto-scale based on demand
- PostgreSQL can be scaled vertically (instance size) or horizontally (read replicas)
- Desktop clients connect independently

## Security Considerations

- API authentication and authorization
- Encrypted communication (HTTPS/WSS)
- Database connection security
- Client-side data encryption for sensitive information
