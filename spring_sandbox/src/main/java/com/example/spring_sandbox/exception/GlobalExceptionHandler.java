package com.example.spring_sandbox.exception;

import jakarta.persistence.EntityNotFoundException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;

import java.time.LocalDateTime;

@ControllerAdvice
public class GlobalExceptionHandler {
    private static final Logger log =
        LoggerFactory.getLogger(GlobalExceptionHandler.class);

    @ExceptionHandler(EntityNotFoundException.class)
    public ResponseEntity<ErrorResponseDto> handleEntityNotFound(
        EntityNotFoundException exception
    ) {
        log.error("Handling EntityNotFoundException: ", exception);

        ErrorResponseDto errorDto = new ErrorResponseDto(
            "Entity not found",
            exception.getMessage(),
            LocalDateTime.now()
        );

        return ResponseEntity
            .status(HttpStatus.NOT_FOUND)
            .body(errorDto);
    }

    @ExceptionHandler(UserAlreadyExistsException.class)
    public ResponseEntity<ErrorResponseDto> handleUserExists(
        UserAlreadyExistsException exception
    ) {
        log.error("Handling UserAlreadyExistsException: ", exception);

        ErrorResponseDto errorDto = new ErrorResponseDto(
            "User already exists",
            exception.getMessage(),
            LocalDateTime.now()
        );

        return ResponseEntity
            .status(HttpStatus.CONFLICT)
            .body(errorDto);
    }

    @ExceptionHandler(Exception.class)
    public ResponseEntity<ErrorResponseDto> handleGenericException(
        Exception exception
    ) {
        log.error("Handling exception: ", exception);

        ErrorResponseDto errorDto = new ErrorResponseDto(
            "Internal server error",
            exception.getMessage(),
            LocalDateTime.now()
        );

        return ResponseEntity
            .status(HttpStatus.INTERNAL_SERVER_ERROR)
            .body(errorDto);
    }
}
