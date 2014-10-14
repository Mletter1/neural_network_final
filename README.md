neural_network_final
====================

Final Project for CS547 Neural Networks

Neural Networks Semester Research Project Description 2014
====================

Project Goals and Rules
====================

The project goal is to design, implement and evaluate a completely neural network-based simulated agent “brain” that learns to live as long as possible in an artificial 2D environment.
The instructor will supply an API to a simulator that supplies sensor information from the agent and sends action commands to the agent. The agent may not be modified in this project. An agent’s performance in the world is quantified by its lifespan measured in simulation time steps. The teams will use the development world to train and test their neural architectures, the results of which will be documented in their final report. All learning and testing must occur within the world, which implies that all learning algorithms will be “on-line” or incremental. Batch learning will not be possible since the entire lifetime of one agent will constitute a single epoch. Not all neural parameters and weights must be learned – it is permitted to preset weights if they do not depend on the objects in the world. Weights learned by one agent may be saved and reused by a new agent. Although all functions of the agent’s brain must be performed with neural networks, it is permitted to use an “algorithmic” version of a neural module if you can show that the algorithm has the exact functionality of the neural module. For example, winner-takes-all network in your design my be implemented with an argmax() function. In the case where substitutions like this are used, the neural network circuit must still be shown in architectural diagrams in the report and a section in an appendix must be supplied to justify the functional equivalence.

The team should plan on creating a sequence of neural architectures that start with the simplest design and progress to more complicated ones as the need arises. For example, Architecture 0 could be the “null” brain, where the agent has no brain, and you measure the default lifespan with no sensing and acting. Architecture 1 might be where the agent moves in some fixed trajectory, like a straight line, and eats everything it contacts. In this case, you would study the distribution of classes of food objects it has eaten in a single trial and measure its lifespan. You would plot that agent’s energy level as a function of simulation time to study how it varies. You would run many trials from different starting points in the world to accumulate a histogram of lifespans, computing the average and standard deviation lifespan for the collection of trials. Each new architecture should be designed to incrementally improve on the last, fixing a single problem that has become apparent. In the final report’s Results section, each architecture will have its own subsection where you present all of the data you have collected that characterizes its performance.

Note:
====================

that a highly rated project report will be one that does a thorough job of characterizing each architecture and performs insightful analysis of their strengths and weaknesses leading to new designs. One would expect that each architecture should have longer lifespans on the average than its predecessor, although the report will not be evaluated on how long the ultimate agent lives.

Code:
====================
Code written by the teams for neural network simulations may be in any language, but must be linkable to the C language simulator. Neural toolboxes or commercial libraries such as those found in Matlab will be permitted, although those teams that implement the networks themselves will receive extra credit.
Teams
￼￼￼
Students will form independent teams of two students each to conduct the research, development, evaluation, and documentation for the project. Teams will be formed by the end of the third week of the semester and reported to the instructor via email. Teams will work totally independent of each other. Fair partitioning of the work within teams will be assured with peer evaluation methods.

Preliminary Design Document:
====================

Mid-semester, a design document will be submitted by each team, consisting of a summary of the architecture, rationale for choices, preliminary results, and initial references.

Research Project Report:
====================

The Monday of finals week, the teams will submit by email a pdf formatted report on their project. The report must thoroughly document the design and testing of the neural network architectures, including theory and dynamics, network diagrams, pseudo code for learning algorithms, performance results, and references. A specific outline format will be supplied.
