//
// Created by jewoo on 2021-11-23.
//
// 그래프
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <deque>
#include <queue>

namespace ch18 {
    using unordered_map_set_string = std::unordered_map<std::string, std::unordered_set<std::string >>;

    struct MatchResult {
        std::string winning_team, losing_team;


        bool CanTeamABeatTeamB(const std::vector<MatchResult> &matches,
                               const std::string &team_a, const std::string &team_b) {
            return IsReachableDFS(BuildGraph(matches), team_a, team_b,
                                  std::make_unique<std::unordered_set<std::string>>().get());
        }

        unordered_map_set_string
        BuildGraph(const std::vector<MatchResult> &matches) {
            unordered_map_set_string graph;
            for (const MatchResult &match: matches) {
                graph[match.winning_team].emplace(match.losing_team);
            }
            return graph;
        }

        bool IsReachableDFS(unordered_map_set_string
                            graph,
                            const std::string &curr,
                            const std::string &dest,
                            std::unordered_set<std::string>
                            *visited_ptr) {
            std::unordered_set<std::string> &visited = *visited_ptr;
            if (curr == dest)
                return true;
            else if (visited.count(curr) || !graph.count(curr))
                return false;
            visited.emplace(curr);
            const std::unordered_set<std::string> &team_list = graph.at(curr);
            return std::any_of(std::begin(team_list), std::end(team_list), [&](const std::string &team) {
                return IsReachableDFS(graph, team, dest, visited_ptr);
            });
        }
    };

    // 1.미로 찾기
    typedef enum {
        kWhite, kBlack
    } Color;

    struct Coordinate {
        bool operator==(const Coordinate &that) const {
            return x == that.x && y == that.y;
        }

        int x, y;

        std::vector<Coordinate> SearchMaze(std::vector<std::vector<Color>> maze,
                                           const Coordinate &s, const Coordinate &e) {
            std::vector<Coordinate> path;
            SearchMazeHelper(s, e, &maze, &path);
            return path;
        }

        bool SearchMazeHelper(const Coordinate &cur, const Coordinate &e,
                              std::vector<std::vector<Color>> *maze_ptr,
                              std::vector<Coordinate> *path_ptr) {
            auto &maze = *maze_ptr;
            //
            if (cur.x < 0 || cur.x >= std::size(maze) ||
                cur.y < 0 || cur.y >= std::size(maze[cur.x]) || maze[cur.x][cur.y] != kWhite) {
                return false;
            }
            auto &path = *path_ptr;
            path.emplace_back(cur);

            maze[cur.x][cur.y] = kBlack;
            if (cur == e)
                return true;

            for (const Coordinate &next_move: {Coordinate{cur.x, cur.y + 1},
                                               Coordinate{cur.x, cur.y - 1},
                                               Coordinate{cur.x + 1, cur.y}, Coordinate{cur.x - 1, cur.y}}) {
                if (SearchMazeHelper(next_move, e, maze_ptr, path_ptr)) {
                    return true;
                }
            }
            path.pop_back();
            return false;
        }

        // 2. 불 행렬 색칠하기
        void FlipColor(int x, int y, std::vector<std::deque<bool>> *image_ptr) {
            std::vector<std::deque<bool>> &image = *image_ptr;

            const bool color = image[x][y];
            std::queue<std::pair<int, int>> q;
            q.emplace(x, y);

            while (!std::empty(q)) {
                const auto[x, y] = q.front();
                q.pop();
                for (const auto &[next_x, next_y]: std::initializer_list<std::pair<int, int>>{
                        {x,     y + 1},
                        {x,     y - 1},
                        {x + 1, y},
                        {x - 1, y}
                }) {
                    if (next_x >= 0 && next_x < std::size(image) && next_y >= 0 &&
                        next_y < std::size(image[next_x]) && image[next_x][next_y] == color) {

                        // 색깔을 뒤집는다.
                        image[next_x][next_y] = !color;
                        q.emplace(next_x, next_y);
                    }
                }
            }
        }

        void FlipColor2(int x, int y, std::vector<std::deque<bool>> *image_ptr) {
            std::vector<std::deque<bool>> &image = *image_ptr;
            const bool color = image[x][y];

            image[x][y] = !color; // 색깔을 뒤집는다.
            for (const auto &[next_x, next_y]: std::initializer_list<std::pair<int, int>>{
                    {x,     y + 1},
                    {x,     y - 1},
                    {x + 1, y},
                    {x - 1, y}
            }) {
                if (next_x >= 0 && next_x < std::size(image) && next_y >= 0 &&
                    next_y < std::size(image[next_x]) && image[next_x][next_y] == color) {
                    FlipColor2(next_x, next_y, &image);
                }
            }
        }

        // 3. 닫힌 지역 찾기
        void FillSurroundedRegions(std::vector<std::vector<char>> *board_ptr) {
            std::vector<std::vector<char>> &board = *board_ptr;

            for (int i = 0; i < std::size(board); ++i) {
                MarkBoundaryRegion(i, 0, board_ptr);
                MarkBoundaryRegion(i, std::size(board[i]) - 1,
                                   board_ptr);
            }
            for (int j = 0; j < std::size(board.front()); ++j) {
                MarkBoundaryRegion(0, j, board_ptr);
                MarkBoundaryRegion(std::size(board) - 1,
                                   j, board_ptr);
            }
            for (std::vector<char> &row: board) {
                for (char &c: row) {
                    c = c != 'T' ? 'B' : 'W';
                }
            }
        }

        void MarkBoundaryRegion(int i, int j,
                                std::vector<std::vector<char>> *board_ptr) {
            std::queue<std::pair<int, int>> q(std::deque<std::pair<int, int>>(1, {i, j}));
            std::vector<std::vector<char>> &board = *board_ptr;

            // 너비 우선 탐색을 사용해서 지역을 찾는다.
            while (!std::empty(q)) {
                const auto[x, y] = q.front();
                q.pop();
                if (x >= 0 && x < std::size(board) && y >= 0 &&
                    y < std::size(board[x]) &&
                    board[x][y] == 'W') {
                    board[x][y] = 'T';
                    q.emplace(x - 1, y);
                    q.emplace(x + 1, y);
                    q.emplace(x, y + 1);
                    q.emplace(x, y - 1);
                }
            }
        }

    };
}

