#pragma once

#include <functional>
#include "../../../components/position.hpp"
#include <rltk.hpp>
#include <map>
#include <memory>

namespace jobs_board {

    struct job_evaluator_base_t;

    using job_board_t = std::map<int, job_evaluator_base_t *>;
    using job_evaluator_t = std::function<void(job_board_t &, rltk::entity_t &, position_t &, job_evaluator_base_t *)>;

    struct job_evaluator_base_t {
        virtual bool has_tag(rltk::entity_t &e)=0;
        virtual void set_tag(rltk::entity_t &e)=0;
        virtual void exec(job_board_t &board, rltk::entity_t &e, position_t &pos)=0;
    };

    template<typename TAG>
    struct job_evaluator_concrete : public job_evaluator_base_t {
        job_evaluator_concrete(job_evaluator_t func) :
            eval_func(func) 
        {}

        // Function that offers a score for the job type
        const job_evaluator_t eval_func;
        
        // Does the tag associated with this work-type exist?
        virtual bool has_tag(rltk::entity_t &e) override final {
            return e.component<TAG>() != nullptr;
        }

        // Assign the tag to the entity
        virtual void set_tag(rltk::entity_t &e) override final {
            e.assign(TAG{});
        }

        // Call the job function
        virtual void exec(job_board_t &board, rltk::entity_t &e, position_t &pos) override final {
            eval_func(board, e, pos, this);
        }
    };

    namespace impl {
        extern std::vector<std::unique_ptr<job_evaluator_base_t>> evaluators;
    }

    template <typename T>
    inline void register_job_offer(job_evaluator_t evaluator) {
        std::unique_ptr<job_evaluator_base_t> base = std::make_unique<job_evaluator_concrete<T>>(evaluator);

        impl::evaluators.emplace_back( std::move(base) );
    }

    bool is_working(rltk::entity_t &e);
    void evaluate(job_board_t &board, rltk::entity_t &entity, position_t &pos);
}