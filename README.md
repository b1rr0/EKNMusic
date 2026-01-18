# EKNMusic

Cross-platform music application with serverless backend and desktop client.

## Project Overview

EKNMusic consists of two main components:

1. **Backend** - Python serverless API (AWS Lambda + PostgreSQL)
2. **Desktop Client** - Qt C++ application (Windows, macOS, Linux)

## Repository Structure

```
EKNMusic/
├── backend/               # Python serverless backend
│   ├── src/              # Source code
│   ├── tests/            # Tests
│   ├── docker/           # Docker configuration
│   └── migrations/       # Database migrations
├── client/               # Qt C++ desktop application
│   ├── src/              # Source code
│   ├── tests/            # Tests
│   └── platform/         # Platform-specific files
├── docs/                 # Project documentation
└── README.md            # This file
```

## Quick Start

### Backend Development

1. **Prerequisites**
   - Python 3.11+
   - Docker Desktop
   - PostgreSQL (via Docker)

2. **Setup**
   ```bash
   cd backend
   python -m venv venv
   source venv/bin/activate  # Windows: venv\Scripts\activate
   pip install -r requirements.txt
   pip install -r requirements-dev.txt
   ```

3. **Start Services**
   ```bash
   cd docker
   docker-compose up -d
   ```

4. **Run Backend**
   ```bash
   python -m uvicorn src.main:app --reload
   ```

See [backend/README.md](backend/README.md) for detailed instructions.

### Desktop Client Development

1. **Prerequisites**
   - C++ compiler (MSVC/GCC/Clang)
   - Qt 6.x SDK
   - CMake 3.16+

2. **Build**
   ```bash
   cd client
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
   cmake --build .
   ```

See [client/README.md](client/README.md) for detailed instructions.

## Documentation

Comprehensive documentation available in the [docs/](docs/) directory:

- [Architecture](docs/architecture.md) - System design and components
- [Backend Setup](docs/backend-setup.md) - Backend configuration
- [Desktop Client](docs/desktop-client.md) - Client build instructions
- [Development Environment](docs/development-environment.md) - Dev setup
- [API Documentation](docs/api-documentation.md) - API reference
- [Database Schema](docs/database-schema.md) - Database design
- [Deployment Guide](docs/deployment-guide.md) - Deployment instructions

## Technology Stack

### Backend
- **Language**: Python 3.11+
- **Framework**: FastAPI / Flask
- **Database**: PostgreSQL 15+
- **Cloud**: AWS Lambda, RDS, S3
- **Local Dev**: Docker

### Desktop Client
- **Language**: C++17/C++20
- **Framework**: Qt 6.x
- **Build**: CMake
- **Platforms**: Windows 10+, macOS 11+, Linux

## Development Workflow

### Backend
```bash
# Start Docker services
cd backend/docker && docker-compose up -d

# Activate virtual environment
cd backend && source venv/bin/activate

# Run migrations
alembic upgrade head

# Start server
uvicorn src.main:app --reload

# Run tests
pytest tests/
```

### Desktop Client
```bash
# Configure build
cd client
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt

# Build
cmake --build .

# Run
./EKNMusic  # or EKNMusic.exe on Windows
```

## Testing

### Backend Tests
```bash
cd backend
pytest tests/ -v --cov=src
```

### Client Tests
```bash
cd client/build
ctest --output-on-failure
```

## Deployment

### Backend to AWS
```bash
cd backend
serverless deploy --stage prod
```

### Desktop Client Distribution
- **Windows**: Create installer with NSIS/Inno Setup
- **macOS**: Create DMG with code signing
- **Linux**: Build AppImage/deb/rpm packages

See [Deployment Guide](docs/deployment-guide.md) for details.

## Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

### Code Standards

**Backend:**
- Follow PEP 8
- Use Black for formatting
- Run pylint and mypy
- Write tests for new features

**Desktop Client:**
- Follow Qt coding conventions
- Use consistent naming (camelCase/PascalCase)
- Write unit tests
- Test on multiple platforms

## Environment Variables

### Backend
Copy `.env.example` to `.env` and configure:
```bash
cd backend
cp .env.example .env
# Edit .env with your settings
```

### Desktop Client
Configure API endpoint in application settings or environment:
```bash
EKNMUSIC_API_URL=https://api.eknmusic.com
```

## Troubleshooting

### Backend Issues
- Database connection: Ensure Docker PostgreSQL is running
- Port conflicts: Change ports in docker-compose.yml
- Import errors: Verify virtual environment is activated

### Client Issues
- Qt not found: Set CMAKE_PREFIX_PATH
- Build errors: Check compiler and Qt version
- Runtime errors: Run deployment tool (windeployqt/macdeployqt)

## Resources

- [Qt Documentation](https://doc.qt.io/)
- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [AWS Lambda Documentation](https://docs.aws.amazon.com/lambda/)
- [PostgreSQL Documentation](https://www.postgresql.org/docs/)

## License

[Specify your license here]

## Support

For issues and questions:
- Open an issue on GitHub
- Check documentation in [docs/](docs/)
- Review component READMEs: [backend](backend/README.md) | [client](client/README.md)
