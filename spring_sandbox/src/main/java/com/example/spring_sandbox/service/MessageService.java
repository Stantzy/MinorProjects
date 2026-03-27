package com.example.spring_sandbox.service;

import com.example.spring_sandbox.dto.request.CreateMessageRequest;
import com.example.spring_sandbox.dto.request.UpdateMessageRequest;
import com.example.spring_sandbox.entity.Message;
import com.example.spring_sandbox.entity.User;
import com.example.spring_sandbox.repository.MessageRepository;
import com.example.spring_sandbox.repository.UserRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import lombok.RequiredArgsConstructor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
@RequiredArgsConstructor
@Transactional
public class MessageService {
    private final Logger log = LoggerFactory.getLogger(MessageService.class);

    private final MessageRepository messageRepository;
    private final UserRepository userRepository;

    public Message createMessage(String username, CreateMessageRequest request) {
        User user = userRepository.findByUsername(username)
            .orElseThrow(() -> new EntityNotFoundException("User not found"));

        Message message = new Message();
        message.setContent(request.getContent());
        message.setUser(user);

        Message saved = messageRepository.save(message);
        log.info("Message created for user: {}", username);

        return saved;
    }

    public List<Message> getUserMessages(String username) {
        User user = userRepository.findByUsername(username)
            .orElseThrow(() -> new EntityNotFoundException("User not found"));

        return messageRepository
            .findByUserIdOrderByCreatedAtDesc(user.getId());
    }

    public Message getMessageById(Long messageId, String username) {
        Message message = messageRepository.findById(messageId)
            .orElseThrow(() -> new EntityNotFoundException("Message not found"));

        if (!message.getUser().getUsername().equals(username)) {
            throw new RuntimeException(
                "You don't have permission to access this message"
            );
        }

        return message;
    }

    public Message updateMessage(
        Long messageId, String username,
        UpdateMessageRequest request
    ) {
        Message message = getMessageById(messageId, username);
        message.setContent(request.getContent());

        Message updated = messageRepository.save(message);
        log.info("Message {} updated by user: {}", messageId, username);

        return updated;
    }

    public void deleteMessage(Long messageId, String username) {
        Message message = getMessageById(messageId, username);
        messageRepository.delete(message);
        log.info("Message {} deleted by user: {}", messageId, username);
    }

    public List<Message> getAllMessages() {
        return messageRepository.findAllOrderByCreatedAtDesc();
    }

    public Page<Message> getAllMessages(Pageable pageable) {
        return messageRepository.findAllByOrderByCreatedAtDesc(pageable);
    }

    public Map<String, Object> getUserMessageStats(String username) {
        User user = userRepository.findByUsername(username)
            .orElseThrow(() -> new EntityNotFoundException("User not found"));

        long count = messageRepository.countByUserId(user.getId());

        Map<String, Object> stats = new HashMap<>();
        stats.put("userId", user.getId());
        stats.put("userName", user.getUsername());
        stats.put("totalMessages", count);

        return stats;
    }
}