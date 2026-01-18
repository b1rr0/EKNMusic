# API Documentation

## Base URL

- **Development**: `http://localhost:8000/api/v1`
- **Staging**: `https://staging-api.eknmusic.com/api/v1`
- **Production**: `https://api.eknmusic.com/api/v1`

## Authentication

### Overview
API uses JWT (JSON Web Tokens) for authentication.

### Authentication Flow

1. Client obtains token via login endpoint
2. Token included in `Authorization` header for subsequent requests
3. Token expires after configured time period
4. Refresh token used to obtain new access token

### Headers

```
Authorization: Bearer <access_token>
Content-Type: application/json
```

## API Endpoints

### Authentication

#### POST /auth/register
Register a new user account.

**Request Body:**
```json
{
  "email": "user@example.com",
  "password": "securepassword",
  "username": "username"
}
```

**Response (201):**
```json
{
  "user_id": "uuid",
  "email": "user@example.com",
  "username": "username",
  "created_at": "2024-01-01T00:00:00Z"
}
```

#### POST /auth/login
Authenticate user and receive tokens.

**Request Body:**
```json
{
  "email": "user@example.com",
  "password": "securepassword"
}
```

**Response (200):**
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "Bearer",
  "expires_in": 3600,
  "user": {
    "user_id": "uuid",
    "email": "user@example.com",
    "username": "username"
  }
}
```

#### POST /auth/refresh
Refresh access token using refresh token.

**Request Body:**
```json
{
  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

**Response (200):**
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "Bearer",
  "expires_in": 3600
}
```

#### POST /auth/logout
Invalidate current token.

**Headers:** Requires authentication

**Response (200):**
```json
{
  "message": "Successfully logged out"
}
```

### User Management

#### GET /users/me
Get current user profile.

**Headers:** Requires authentication

**Response (200):**
```json
{
  "user_id": "uuid",
  "email": "user@example.com",
  "username": "username",
  "created_at": "2024-01-01T00:00:00Z",
  "updated_at": "2024-01-01T00:00:00Z"
}
```

#### PUT /users/me
Update current user profile.

**Headers:** Requires authentication

**Request Body:**
```json
{
  "username": "newusername",
  "email": "newemail@example.com"
}
```

**Response (200):**
```json
{
  "user_id": "uuid",
  "email": "newemail@example.com",
  "username": "newusername",
  "updated_at": "2024-01-01T00:00:00Z"
}
```

#### DELETE /users/me
Delete current user account.

**Headers:** Requires authentication

**Response (204):** No content

### Music Resources (Example Endpoints)

#### GET /music/tracks
List user's music tracks.

**Headers:** Requires authentication

**Query Parameters:**
- `page` (integer, default: 1)
- `limit` (integer, default: 20, max: 100)
- `sort` (string: "created_at", "title", "artist")
- `order` (string: "asc", "desc")

**Response (200):**
```json
{
  "tracks": [
    {
      "track_id": "uuid",
      "title": "Song Title",
      "artist": "Artist Name",
      "album": "Album Name",
      "duration": 240,
      "file_url": "https://...",
      "created_at": "2024-01-01T00:00:00Z"
    }
  ],
  "pagination": {
    "page": 1,
    "limit": 20,
    "total_pages": 5,
    "total_items": 100
  }
}
```

#### POST /music/tracks
Upload a new music track.

**Headers:**
- Requires authentication
- `Content-Type: multipart/form-data`

**Request Body:**
- `file` (binary): Audio file
- `title` (string)
- `artist` (string)
- `album` (string, optional)

**Response (201):**
```json
{
  "track_id": "uuid",
  "title": "Song Title",
  "artist": "Artist Name",
  "album": "Album Name",
  "duration": 240,
  "file_url": "https://...",
  "created_at": "2024-01-01T00:00:00Z"
}
```

#### GET /music/tracks/{track_id}
Get specific track details.

**Headers:** Requires authentication

**Response (200):**
```json
{
  "track_id": "uuid",
  "title": "Song Title",
  "artist": "Artist Name",
  "album": "Album Name",
  "duration": 240,
  "file_url": "https://...",
  "metadata": {
    "bitrate": 320,
    "format": "mp3"
  },
  "created_at": "2024-01-01T00:00:00Z"
}
```

#### DELETE /music/tracks/{track_id}
Delete a music track.

**Headers:** Requires authentication

**Response (204):** No content

## Error Responses

### Standard Error Format

```json
{
  "error": {
    "code": "ERROR_CODE",
    "message": "Human-readable error message",
    "details": {}
  }
}
```

### HTTP Status Codes

- `200 OK` - Request successful
- `201 Created` - Resource created successfully
- `204 No Content` - Request successful, no content to return
- `400 Bad Request` - Invalid request parameters
- `401 Unauthorized` - Authentication required or failed
- `403 Forbidden` - Authenticated but not authorized
- `404 Not Found` - Resource not found
- `409 Conflict` - Resource conflict (e.g., duplicate)
- `422 Unprocessable Entity` - Validation errors
- `429 Too Many Requests` - Rate limit exceeded
- `500 Internal Server Error` - Server error
- `503 Service Unavailable` - Service temporarily unavailable

### Common Error Codes

- `INVALID_CREDENTIALS` - Login credentials incorrect
- `TOKEN_EXPIRED` - Access token has expired
- `TOKEN_INVALID` - Access token is invalid
- `RESOURCE_NOT_FOUND` - Requested resource doesn't exist
- `VALIDATION_ERROR` - Request validation failed
- `DUPLICATE_RESOURCE` - Resource already exists
- `RATE_LIMIT_EXCEEDED` - Too many requests

## Rate Limiting

- Rate limits applied per user/IP address
- Limits: 100 requests per minute
- Rate limit info in response headers:
  - `X-RateLimit-Limit`: Maximum requests per window
  - `X-RateLimit-Remaining`: Remaining requests
  - `X-RateLimit-Reset`: Unix timestamp when limit resets

## Pagination

List endpoints support pagination:

**Query Parameters:**
- `page`: Page number (default: 1)
- `limit`: Items per page (default: 20, max: 100)

**Response:**
```json
{
  "items": [...],
  "pagination": {
    "page": 1,
    "limit": 20,
    "total_pages": 5,
    "total_items": 100,
    "has_next": true,
    "has_prev": false
  }
}
```

## Versioning

API version specified in URL path: `/api/v1/`

Future versions will maintain backward compatibility or provide migration paths.

## WebSocket API (Optional)

If real-time features needed:

### Connection
```
wss://api.eknmusic.com/ws?token=<access_token>
```

### Message Format
```json
{
  "type": "message_type",
  "payload": {}
}
```

## SDK / Client Libraries

Desktop client implements API client wrapper for:
- Automatic token management
- Request retry logic
- Error handling
- Offline queue support
