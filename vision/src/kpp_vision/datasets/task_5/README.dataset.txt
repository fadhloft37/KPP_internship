# Hollow Knight > hollow-knight-3rnuv
https://universe.roboflow.com/zicos-workspace/hollow-knight-3rnuv

Provided by a Roboflow user
License: CC BY 4.0

### **Project Description for Roboflow**

**Project Title**: Hollow Knight Object Detection for Reinforcement Learning Agent

**Description**:  
This project focuses on developing an object detection model tailored to the popular game *Hollow Knight*. The goal is to detect and classify various in-game elements in real-time to create a dataset that powers a reinforcement learning (RL) agent. This agent will use the detected objects as inputs to interact with the game environment, make decisions, and achieve specific objectives such as defeating enemies, collecting items, and progressing through the game.

The object detection model will classify key elements in the game into the following **10 classes**:  

1. **Mana**: Represents the player's magic reserve, displayed as a circular indicator that fills with white. Additionally, smaller circles may appear, representing extra magic capacity.  
2. **Health Bar**: Displays the player’s health as a series of masks that fill with white. Each mask corresponds to one unit of health.  
3. **HK (Hollow Knight)**: The main character's position, representing the player’s sprite on the screen.  
4. **Enemy**: Any hostile character or entity that can attack or be attacked by the player. Includes grounded, flying, and stationary enemies.  
5. **Collectible Item**: Objects that can be picked up or interacted with to provide rewards such as Geo (currency), life fountains, or station stops.  
6. **Bench**: Resting spots where the player can save progress and heal.  
7. **Upgrade Item**: Rare collectibles that permanently enhance abilities or stats, such as health or mana upgrades.  
8. **Key Item**: Special objects necessary for game progression, such as keys or crests.  
9. **Exit**: Doorways, breakable barriers, or hidden passages that transition the player to new areas.  
10. **NPC**: Non-hostile characters that the player can interact with for trading, quests, or story progression.

The object detection system will enable the RL agent to process and interpret the game environment, enabling intelligent decision-making.

---

### **Purpose and Objectives**:

1. **Object Detection**:  
   Develop a robust YOLO-based object detection model to identify and classify game elements from video frames.  

2. **Reinforcement Learning (RL)**:  
   Utilize the outputs of the object detection system (e.g., bounding boxes and class predictions) as the **state inputs** for an RL algorithm. The RL agent will learn to perform tasks such as:  
   - Navigating through the game world.  
   - Interacting with NPCs.  
   - Avoiding or defeating enemies.  
   - Collecting items and managing resources like health and mana.  

3. **Dynamic Adaptation**:  
   Begin training the RL agent with a limited dataset of annotated images, gradually expanding the dataset to improve model performance and adaptability as more scenarios are introduced.

4. **Automation**:  
   The ultimate goal is to automate the gameplay of *Hollow Knight*, enabling the agent to mimic human-like decision-making.

---

### **Integration Plan**:

1. **Object Detection Training**:  
   Use Roboflow for data preprocessing, annotation, augmentation, and model training. Generate a YOLO-compatible dataset and fine-tune the model for detecting the 10 classes.

2. **Reinforcement Learning Agent**:  
   Implement a deep RL algorithm (e.g., **Deep Q-Networks (DQN)** or **Proximal Policy Optimization (PPO)**).  
   - **State Input**: The bounding boxes and class probabilities from the object detection model.  
   - **Actions**: Movement (e.g., left, right, jump), interactions (e.g., attack, heal, cast spells), and resource management.  
   - **Rewards**: Positive rewards for objectives like collecting items or defeating enemies, and negative rewards for losing health or failing objectives.

3. **Feedback Loop**:  
   The RL agent's actions will be fed back into the game, generating new frames that the object detection model processes, creating a closed loop for training and evaluation.