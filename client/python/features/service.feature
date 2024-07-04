# Created by rakesh at 26/09/2023
Feature: TCP Service
  Test client against encrypter TCP service.

  Scenario: Service
    Given The encrypter service running on localhost on port 2030
    Then Encrypt a string
    And Decrypt the encrypted string
    And Encrypt some bytes
    And Decrypt the encrypted string back to bytes
