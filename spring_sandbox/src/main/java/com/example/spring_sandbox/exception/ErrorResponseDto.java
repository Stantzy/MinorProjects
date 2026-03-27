package com.example.spring_sandbox.exception;

import lombok.RequiredArgsConstructor;

import java.time.LocalDateTime;

@RequiredArgsConstructor
public class ErrorResponseDto {
    private final String message;
    private final String detailedMessage;
    private final LocalDateTime errorTime;
}