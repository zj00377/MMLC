# Multi-Resemblance Multi-Target Low-Rank Coding (MMLC)

Alzheimer's Disease (AD) is the most common type of dementia. Identifying correct biomarkers may determine pre-symptomatic AD subjects and enable early intervention. Sparse coding (SC) has shown strong statistical power in many biomedical informatics and brain image analysis researches. However, the SC computation is time-consuming and often leads to inconsistent codes, i.e., local features with similar descriptors tend to have different sparse codes, and longitudinal analysis always contains incomplete data and missing label information. To address above challenges, we invent a novel two-stage Multi-Resemblance Multi-Target Low-Rank Coding (MMLC), which encourages that sparse codes of neighboring time point longitudinal features to be resemblant to each other and only a few sparse codes are necessary to represent all features in a local region to reduce the computational cost. In stage one, we propose a online multi-resemblant low-rank SC method to utilize the common and task-specific dictionaries in different time points data to capture the multiple time points longitudinal correlation. In stage two, supported by a rigorous theoretical analysis, we develop a multi-target learning method to solve the missing label problem. To solve such a multi-task sparse low-rank optimization problem, we propose a stochastic coordinate coding method with a sequence of closed-form update steps which reduce the computational cost guaranteed by a theoretical fast convergence proof. 

## Usage



## Authors: 
	Jie Zhang   Jiezhang.Joena@asu.edu
        Yalin Wang  ylwang@asu.edu
