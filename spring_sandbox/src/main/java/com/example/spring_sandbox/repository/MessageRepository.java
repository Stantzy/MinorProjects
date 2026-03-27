package com.example.spring_sandbox.repository;

import com.example.spring_sandbox.entity.Message;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface MessageRepository extends JpaRepository<Message, Long> {
    @Query("SELECT m FROM Message m ORDER BY m.createdAt DESC")
    List<Message> findAllOrderByCreatedAtDesc();

    List<Message> findByUserIdOrderByCreatedAtDesc(Long userId);

    Page<Message> findByUserId(Long userId, Pageable pageable);

    Page<Message> findAllByOrderByCreatedAtDesc(Pageable pageable);

    long countByUserId(Long userId);

    void deleteByUserId(Long userId);
}