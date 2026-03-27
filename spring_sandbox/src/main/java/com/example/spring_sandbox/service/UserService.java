package com.example.spring_sandbox.service;

import com.example.spring_sandbox.dto.UserDto;
import com.example.spring_sandbox.dto.request.RegistrationRequest;
import com.example.spring_sandbox.exception.UserAlreadyExistsException;
import com.example.spring_sandbox.entity.User;
import com.example.spring_sandbox.dto.request.UserCreateRequest;
import com.example.spring_sandbox.repository.UserRepository;
import jakarta.persistence.EntityNotFoundException;
import lombok.RequiredArgsConstructor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
@RequiredArgsConstructor
public class UserService {
    Logger log = LoggerFactory.getLogger(UserService.class);

    private final PasswordEncoder passwordEncoder;
    private final UserRepository userRepository;

    public List<User> getAllUsers() {
        return userRepository.findAll();
    }

    public UserDto getUserById(Long id) {
        log.info("Called method getUserById: {}", id);

        User user = userRepository.findById(id)
            .orElseThrow(() ->
                new EntityNotFoundException("Not found user by id = " + id)
            );

        return new UserDto(
            user.getId(),
            user.getUsername()
        );
    }

    public User createUser(UserCreateRequest userToCreate) {
        log.info(
            "Called method createUser: username = {}",
            userToCreate.username()
        );

        User user = new User();
        user.setId(null);
        user.setUsername(userToCreate.username());

        User createdUser = userRepository.save(user);

        log.info(
            "User created: id = {}, username = {}",
            createdUser.getId(),
            createdUser.getUsername()
        );

        return createdUser;
    }

    public UserDto registerUser(RegistrationRequest request) {
        log.info(
            "Called method registerUser: username={}",
            request.getUsername()
        );

        if(userRepository.existsByUsername(request.getUsername())) {
            throw new UserAlreadyExistsException(
                "User with username " + request.getUsername()
                    + " already exists"
            );
        }

        User user = new User();
        user.setUsername(request.getUsername());
        user.setPassword(passwordEncoder.encode(request.getPassword()));
        user = userRepository.save(user);


        return new UserDto(
            user.getId(),
            user.getUsername()
        );
    }

    public UserDto updateUserById(Long id, UserDto userDataToUpdate) {
        log.info(
            "Called method updateUserById: id={}, UserDto={}, newUsername={}",
            id,
            userDataToUpdate,
            userDataToUpdate.getUsername()
        );

        User userToUpdate = new User();
        userToUpdate.setId(id);
        userToUpdate.setUsername(userDataToUpdate.getUsername());

        User updatedUser = userRepository.save(userToUpdate);

        return new UserDto(
            updatedUser.getId(),
            updatedUser.getUsername()
        );
    }

    public boolean deleteUserById(Long id) {
        log.info("Called method deleteUserById: id={}", id);

        if(userRepository.existsById(id)) {
            userRepository.deleteById(id);
            return true;
        }

        return false;
    }
}
