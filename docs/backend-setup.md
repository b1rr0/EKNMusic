# Backend Setup

## Technology Stack

- **Language**: Python 3.11+
- **Framework**: TBD (FastAPI, Flask, or Django)
- **Database**: PostgreSQL 15+
- **Serverless**: AWS Lambda
- **API Gateway**: AWS API Gateway or Application Load Balancer
- **Infrastructure**: AWS SAM or Serverless Framework

## Local Development with Docker

### Prerequisites

- Docker Desktop installed
- Docker Compose installed
- Python 3.11+ installed locally

### Directory Structure

```
backend/
├── src/
│   ├── handlers/          # Lambda function handlers
│   ├── models/            # Database models
│   ├── services/          # Business logic
│   └── utils/             # Utility functions
├── tests/
├── docker/
│   └── docker-compose.yml
├── migrations/            # Database migrations
├── requirements.txt
├── requirements-dev.txt
└── serverless.yml         # Serverless configuration
```

### Docker Compose Configuration

The `docker-compose.yml` should include:
- PostgreSQL database service
- Python application service
- Optional: pgAdmin for database management

### Environment Variables

Create `.env` file for local development:
```
DATABASE_HOST=localhost
DATABASE_PORT=5432
DATABASE_NAME=eknmusic_dev
DATABASE_USER=postgres
DATABASE_PASSWORD=your_password
AWS_REGION=us-east-1
```

### Running Locally

```bash
# Start services
docker-compose up -d

# Run migrations
python manage.py migrate

# Start development server
python manage.py runserver
```

## AWS Deployment

### Required AWS Services

- AWS Lambda
- Amazon RDS (PostgreSQL)
- API Gateway
- IAM Roles and Policies
- CloudWatch (Logging)
- Secrets Manager (Database credentials)

### Deployment Steps

1. Configure AWS credentials
2. Set up RDS PostgreSQL instance
3. Configure VPC and security groups
4. Deploy Lambda functions using SAM/Serverless
5. Configure API Gateway routes
6. Run database migrations on RDS
7. Configure environment variables in Lambda

### Environment-Specific Settings

- **Development**: Local Docker environment
- **Staging**: AWS infrastructure with test database
- **Production**: AWS infrastructure with production database and monitoring

## Database Management

### Migrations

Use Alembic (SQLAlchemy) or Django migrations for schema versioning.

### Backup Strategy

- Automated RDS snapshots
- Point-in-time recovery enabled
- Cross-region backup replication for production

## Monitoring and Logging

- CloudWatch Logs for Lambda functions
- CloudWatch Metrics for performance monitoring
- AWS X-Ray for distributed tracing
- Database query performance monitoring
