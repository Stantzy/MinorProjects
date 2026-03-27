package com.example.spring_sandbox.controller;

import com.example.spring_sandbox.dto.request.RegistrationRequest;
import com.example.spring_sandbox.dto.UserDto;
import com.example.spring_sandbox.service.UserService;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;

@RestController
@RequestMapping("/api/auth")
@RequiredArgsConstructor
public class AuthController {
    private final static Logger log =
        LoggerFactory.getLogger(AuthController.class);

    private final UserService userService;

    @PostMapping("/register")
    public ResponseEntity<UserDto> register(
        @RequestBody @Valid RegistrationRequest request
    ) {
        log.info(
            "Registration attempt for username: {}",
            request.getUsername()
        );

        UserDto registeredUser = userService.registerUser(request);

        URI location = ServletUriComponentsBuilder
            .fromCurrentContextPath()
            .pathSegment("api", "users", "{id}")
            .buildAndExpand(registeredUser.getId())
            .toUri();

        return ResponseEntity
            .created(location)
            .body(registeredUser);
    }
}
