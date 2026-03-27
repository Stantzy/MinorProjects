package com.example.spring_sandbox.controller;

import com.example.spring_sandbox.dto.MessageDto;
import com.example.spring_sandbox.dto.request.CreateMessageRequest;
import com.example.spring_sandbox.dto.request.UpdateMessageRequest;
import com.example.spring_sandbox.entity.Message;
import com.example.spring_sandbox.mapper.MessageMapper;
import com.example.spring_sandbox.service.MessageService;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/messages")
@RequiredArgsConstructor
public class MessageController {
    private final Logger log = LoggerFactory.getLogger(MessageController.class);

    private final MessageService messageService;
    private final MessageMapper mapper;

    @PostMapping
    public ResponseEntity<MessageDto> createMessage(
        @RequestBody @Valid CreateMessageRequest request,
        @AuthenticationPrincipal UserDetails userDetails) {

        log.info("Creating message for user: {}", userDetails.getUsername());

        Message message = messageService.createMessage(userDetails.getUsername(), request);

        URI location = ServletUriComponentsBuilder
            .fromCurrentRequest()
            .path("/{id}")
            .buildAndExpand(message.getId())
            .toUri();

        return ResponseEntity
            .created(location)
            .body(mapper.toDto(message));
    }

    @GetMapping("/me")
    public ResponseEntity<List<MessageDto>> getMyMessages(
        @AuthenticationPrincipal UserDetails userDetails) {

        List<Message> messages = messageService.getUserMessages(userDetails.getUsername());
        return ResponseEntity.ok(mapper.toDtoList(messages));
    }

    @GetMapping("/{id}")
    public ResponseEntity<MessageDto> getMessage(
        @PathVariable Long id,
        @AuthenticationPrincipal UserDetails userDetails) {

        Message message = messageService.getMessageById(id, userDetails.getUsername());
        return ResponseEntity.ok(mapper.toDto(message));
    }

    @PutMapping("/{id}")
    public ResponseEntity<MessageDto> updateMessage(
        @PathVariable Long id,
        @RequestBody @Valid UpdateMessageRequest request,
        @AuthenticationPrincipal UserDetails userDetails) {

        Message updated = messageService.updateMessage(id, userDetails.getUsername(), request);
        return ResponseEntity.ok(mapper.toDto(updated));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteMessage(
        @PathVariable Long id,
        @AuthenticationPrincipal UserDetails userDetails) {

        messageService.deleteMessage(id, userDetails.getUsername());
        return ResponseEntity.noContent().build();
    }

    @GetMapping("/feed")
    public ResponseEntity<List<MessageDto>> getFeed(
        @RequestParam(defaultValue = "0") int page,
        @RequestParam(defaultValue = "20") int size) {

        Pageable pageable = PageRequest.of(page, size, Sort.by("createdAt").descending());
        Page<Message> messages = messageService.getAllMessages(pageable);

        return ResponseEntity.ok(mapper.toDtoList(messages.getContent()));
    }

    @GetMapping("/stats")
    public ResponseEntity<Map<String, Object>> getStats(
        @AuthenticationPrincipal UserDetails userDetails) {

        return ResponseEntity.ok(messageService.getUserMessageStats(userDetails.getUsername()));
    }
}