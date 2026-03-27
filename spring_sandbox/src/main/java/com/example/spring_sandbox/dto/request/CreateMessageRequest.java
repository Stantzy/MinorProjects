package com.example.spring_sandbox.dto.request;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class CreateMessageRequest {
    @NotBlank(message = "Content cannot be empty")
    @Size(max = 1000, message = "Content must be less than 1000 characters")
    private String content;
}
