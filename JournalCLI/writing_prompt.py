import random

writing_prompts = [
    "Describe a memorable event from your childhood.",
    "Write about a person who has had a significant impact on your life.",
    "What is your favorite place in the world? Describe it in detail.",
    "If you could have any superpower, what would it be and why?",
    "Write about a time when you overcame a challenge or obstacle.",
    "If you could travel back in time, which historical event would you like to witness?",
    "Describe a dream you've had that left a lasting impression on you.",
    "What is your favorite book or movie? Explain why it's meaningful to you.",
    "If you could spend a day with any fictional character, who would it be and what would you do?",
    "Write about a favorite family tradition.",
    "Describe your perfect day from start to finish.",
    "What is your favorite hobby? How did you get into it?",
    "If you could live in any time period, when would it be and why?",
    "Write about a person who has inspired you.",
    "What is something you've always wanted to learn or try?",
    "If you could switch lives with someone for a day, who would it be and why?",
    "What are your long-term goals? What steps are you taking to achieve them?",
    "Write about a time when you took a risk and what you learned from the experience.",
    "Describe an object that holds sentimental value to you.",
    "What are three things you're grateful for today?",
]

random_prompt = random.choice(writing_prompts)
print(random_prompt)