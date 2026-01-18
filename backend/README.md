# EKNMusic Backend

Python serverless backend for EKNMusic application.

## Technology Stack

- **Language**: Python 3.11+
- **Framework**: FastAPI / Flask / Django (TBD)
- **Database**: PostgreSQL 15+
- **Deployment**: AWS Lambda (Serverless)
- **Local Development**: Docker

## Directory Structure

```
backend/
├── src/
│   ├── handlers/          # Lambda function handlers
│   ├── models/            # Database models (SQLAlchemy/Django ORM)
│   ├── services/          # Business logic layer
│   └── utils/             # Utility functions and helpers
├── tests/                 # Unit and integration tests
├── docker/                # Docker configuration files
│   └── docker-compose.yml
├── migrations/            # Database migration files
├── requirements.txt       # Production dependencies
├── requirements-dev.txt   # Development dependencies
├── serverless.yml         # Serverless Framework configuration
├── .env.example          # Environment variables template
└── README.md             # This file
```

## Quick Start

### Local Development with Docker

1. **Install Prerequisites**
   - Docker Desktop
   - Python 3.11+
   - pip

2. **Set up Environment**
   ```bash
   cd backend
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   pip install -r requirements.txt
   pip install -r requirements-dev.txt
   ```

3. **Configure Environment Variables**
   ```bash
   cp .env.example .env
   # Edit .env with your settings
   ```

4. **Start Services**
   ```bash
   cd docker
   docker-compose up -d
   ```

5. **Run Migrations**
   ```bash
   # Run database migrations
   alembic upgrade head
   ```

6. **Start Development Server**
   ```bash
   python -m uvicorn src.main:app --reload
   # Or for Flask: python src/main.py
   ```

## Development

### Running Tests
```bash
pytest tests/
pytest tests/ -v --cov=src
```

### Code Quality
```bash
# Linting
pylint src/
flake8 src/

# Formatting
black src/
isort src/

# Type checking
mypy src/
```

### Database Migrations
```bash
# Create new migration
alembic revision --autogenerate -m "description"

# Apply migrations
alembic upgrade head

# Rollback migration
alembic downgrade -1
```

## Deployment

### Deploy to AWS
```bash
# Deploy to development
serverless deploy --stage dev

# Deploy to production
serverless deploy --stage prod
```

See [deployment guide](../docs/deployment-guide.md) for detailed instructions.

## API Documentation

Once the server is running, visit:
- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc

## Environment Variables

See `.env.example` for all required environment variables.

Key variables:
- `DATABASE_URL`: PostgreSQL connection string
- `AWS_REGION`: AWS region for deployment
- `JWT_SECRET_KEY`: Secret for JWT token generation
- `ENVIRONMENT`: dev/staging/prod

## Contributing

1. Create feature branch from `develop`
2. Write tests for new features
3. Ensure all tests pass
4. Format code with black/isort
5. Submit pull request

## License

[License information]
