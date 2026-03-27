package com.example.spring_sandbox.controller;

import com.example.spring_sandbox.dto.UserDto;
import com.example.spring_sandbox.dto.request.UserCreateRequest;
import com.example.spring_sandbox.dto.response.UserCreateResponse;
import com.example.spring_sandbox.entity.User;
import com.example.spring_sandbox.service.UserService;
import lombok.RequiredArgsConstructor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;

@RestController
@RequestMapping("/api/users")
@RequiredArgsConstructor
public class UserController {
    private final static Logger log =
        LoggerFactory.getLogger(UserController.class);

    private final UserService userService;

    @GetMapping
    public ResponseEntity<List<User>> getAllUsers() {
        log.info("Called method getAllUsers");

        List<User> users = userService.getAllUsers();

        return ResponseEntity.ok(users);
    }

    @GetMapping("/{id}")
    public ResponseEntity<UserDto> getUser(
        @PathVariable Long id
    ) {
        log.info("Called method getUser: id={}", id);

        UserDto userDto = userService.getUserById(id);

        return ResponseEntity.status(HttpStatus.OK).body(userDto);
    }

    @PostMapping
    public ResponseEntity<UserCreateResponse> createUser(
        @RequestBody UserCreateRequest userToCreate
    ) {
        log.info(
            "Called method createUser: username={}",
            userToCreate.username()
        );

        User createdUser = userService.createUser(userToCreate);
        UserCreateResponse userCreateResponse = new UserCreateResponse(
            createdUser.getId(),
            createdUser.getUsername()
        );

        URI location = getLocationOfCreatedUser(createdUser);

        return ResponseEntity.created(location)
            .body(userCreateResponse);
    }

    @PutMapping("/{id}")
    public ResponseEntity<UserDto> updateUser(
        @PathVariable Long id,
        @RequestBody UserDto userUpdateRequest
    ) {
        log.info("Called method updateUser: id={}", id);

        UserDto updatedUser =
            userService.updateUserById(id, userUpdateRequest);

        return ResponseEntity.ok(updatedUser);
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteUser(@PathVariable Long id) {
        log.info("Called method deleteUser: id={}", id);

        boolean deleted = userService.deleteUserById(id);

        if(deleted)
            return ResponseEntity.noContent().build();

        return ResponseEntity.notFound().build();
    }

    private URI getLocationOfCreatedUser(User user) {
        return ServletUriComponentsBuilder
            .fromCurrentRequest()
            .path("/{id}")
            .build(user.getId());
    }
}
