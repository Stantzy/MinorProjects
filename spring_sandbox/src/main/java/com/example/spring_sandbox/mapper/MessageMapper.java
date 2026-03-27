package com.example.spring_sandbox.mapper;

import com.example.spring_sandbox.dto.MessageDto;
import com.example.spring_sandbox.entity.Message;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.stream.Collectors;

@Component
public class MessageMapper {
    public MessageDto toDto(Message message) {
        if (message == null) return null;

        return new MessageDto(
            message.getId(),
            message.getContent(),
            message.getUser().getId(),
            message.getUser().getUsername(),
            message.getCreatedAt(),
            message.getUpdatedAt()
        );
    }

    public List<MessageDto> toDtoList(List<Message> messages) {
        return messages.stream()
            .map(this::toDto)
            .collect(Collectors.toList());
    }
}